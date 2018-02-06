#include "oauth2.h"
#include <QJsonDocument>
#include <QJsonObject>


OAuth2::OAuth2(QObject *parent) : QObject(parent)
{
    mResponseServer = new QTcpServer( this );
    mNetworkAccessManager = new QNetworkAccessManager( this );

    connect( mResponseServer, &QTcpServer::newConnection, this, &OAuth2::onIncomingConnection );
}

bool OAuth2::authenticate( const QString& authenticationUrl,
                    const QString& tokenExchangeUrl,
                    const QString& clientID,
                    const QString& clientSecret,
                    const QString& scope,
                    const QString& state,
                    const QString& loginHint )
{
    if( mAuthState != AuthState::nothing )
    {
        log( "Request aborted as something else is currently in progress..." );
        return false;
    }

    // Start the listening server
    if( !mResponseServer->isListening() )
    {
        if( !mResponseServer->listen( QHostAddress( mResponseAddress ) ) )
        {
            handleAuthError( QString( "Unable to start listening on %1 (%2)..." ).arg( mResponseAddress ).arg( mResponseServer->errorString() ) );
            return false;
        }
        mResponsePort = mResponseServer->serverPort();
    }

    mAuthQueryUrls.mAuthenticationUrl   = authenticationUrl;
    mAuthQueryUrls.mTokenExchangeUrl    = tokenExchangeUrl;
    mAuthQueryDetails.clientID          = clientID;
    mAuthQueryDetails.clientSecret      = clientSecret;
    mAuthQueryDetails.scope             = scope;
    mAuthQueryDetails.state             = state;
    mAuthQueryDetails.loginHint         = loginHint;
    mAuthQueryDetails.codeVerifier     = generateCodeVerifier();
    mAuthQueryDetails.codeChallenge    = generateCodeChallenge( mAuthQueryDetails.codeVerifier );

    startAuthRequestStep();
    return true;
}

QString OAuth2::errorString()
{
    return mLastError;
}

OAuth2::AccessToken OAuth2::accessToken()
{
    return mAccessToken;
}

void OAuth2::onIncomingConnection()
{
    while( mResponseServer->hasPendingConnections() )
    {
        auto pendingConnection = mResponseServer->nextPendingConnection();

        if( mAuthState == AuthState::waiting_for_user_prompt )
        {
            log( "New incoming connection..." );
            connect( pendingConnection, &QTcpSocket::readyRead, this, &OAuth2::onConnectionReadyRead );
            connect( pendingConnection, &QTcpSocket::disconnected, this, &OAuth2::onConnectionDisconnected );
            connect( pendingConnection, QOverload<QAbstractSocket::SocketError>::of( &QTcpSocket::error ), this, &OAuth2::onConnectionError );
        }

        else
        {
            // Kill any connection attempts we aren't expecting
            log( "Terminating unexpected incoming connection..." );
            pendingConnection->close();
            pendingConnection->deleteLater();
        }
    }
}

void OAuth2::onConnectionReadyRead()
{
    auto connection = qobject_cast<QTcpSocket*>( QObject::sender() );

    if( mAuthState == AuthState::waiting_for_user_prompt )
    {
        auto data = connection->readAll();
        QString dataString = QString::fromUtf8( data );
        processPromptResponse( dataString );
        connection->close();
        connection->deleteLater();
    }

    else
    {
        // Kill any connections that we aren't expecting
        log( "Terminating unexpected incoming connection (readyRead)..." );
        connection->close();
        connection->deleteLater();
    }
}

void OAuth2::onConnectionDisconnected()
{
    auto connection = qobject_cast<QTcpSocket*>( QObject::sender() );

    // If we are still waiting for the users response then this is an error...
    if( mAuthState == AuthState::waiting_for_user_prompt )
    {
        handleAuthError( QString( "Socket disconnected: %1..." ).arg( connection->errorString() ) );
        mAuthState = AuthState::nothing;
    }

    connection->deleteLater();
}

void OAuth2::onConnectionError( QAbstractSocket::SocketError socketError )
{
    auto connection = qobject_cast<QTcpSocket*>( QObject::sender() );

    // If we are still waiting for the users response then this is an error...
    if( mAuthState == AuthState::waiting_for_user_prompt )
    {
        handleAuthError( QString( "Socket error: %1 (%2)..." ).arg( connection->errorString() ).arg( socketError ) );
        mAuthState = AuthState::nothing;
    }

    connection->deleteLater();
}

void OAuth2::onReplyError( QNetworkReply::NetworkError code )
{
    auto reply = qobject_cast<QNetworkReply*>( QObject::sender() );

    handleAuthError( QString( "Network reply error: %1 (code: %2)..." ).arg( reply->errorString() ).arg( code ) );
    reply->deleteLater();
    mAuthState = AuthState::nothing;
}

void OAuth2::onReplyFinished()
{
    auto reply = qobject_cast<QNetworkReply*>( QObject::sender() );

    auto data = reply->readAll();

    if( mAuthState == AuthState::waiting_for_auth_request_response )
    {
        log( QString( "Got auth request response HTML (%1 bytes)..." ).arg( data.size() ) );
        startUserPromptStep( data );
    }

    else if( mAuthState == AuthState::waiting_for_token_exchange )
    {
        log( QString( "Got token exchange response (%1 bytes)..." ).arg( data.size() ) );

        AccessToken accessToken;
        if( !processAccessTokenResponse( data, accessToken ) )
        {
            mAuthState = AuthState::nothing;
            return;
        }

        log( "Authentication complete!" );
        mAccessToken = accessToken;
        emit authenticated();
        mAuthState = AuthState::nothing;

        // TODO: should we have a state for authenticated?
    }

    reply->deleteLater();
}

void OAuth2::onReplySslErrors( const QList<QSslError>& errors )
{
    Q_UNUSED( errors );
    auto reply = qobject_cast<QNetworkReply*>( QObject::sender() );

    for( auto error : errors )
        log( QString( "SSL error: %1 (code: %2)..." ).arg( error.errorString() ).arg( error.error() ) );

    // TODO: delete reply????
    // TODO: error????
}

void OAuth2::handleAuthError( QString error )
{
    mLastError = error;
    emit log( error );
    emit authenticationError();
}

bool OAuth2::getVariableFromString( const QString& variableName, const QString& string, QString& value_OUT )
{
    // finds the specified variable in string and copies its value to value_OUT
    // returns false if it can't find the variable

    QString variablePrefix = variableName + "=";
    int index = string.indexOf( variablePrefix );

    if( index == -1 )
        return false;

    index = index + variablePrefix.length();
    for( int i = index; i < string.length(); i++ )
    {
        QChar c = string[ i ];
        if( c.isSpace() )
            break;

        value_OUT += c;
    }

    return true;
}

void OAuth2::processPromptResponse( const QString& data )
{
    log( QString( "Got user prompt response: %1" ).arg( data ) );

    const QString codeVariableName = "code";
    const QString errorVariableName = "error";

    // Check for an error in the response
    QString error;
    if( getVariableFromString( errorVariableName, data, error ) )
    {
        handleAuthError( QString( "Error detected in the user prompt response: %1..." ).arg( error ) );
        mAuthState = AuthState::nothing;
        return;
    }

    // No error, so check for a code in the response
    QString code;
    if( !getVariableFromString( codeVariableName, data, code ) )
    {
        handleAuthError( "No code present in the user prompt response..." );
        mAuthState = AuthState::nothing;
        return;
    }

    if( !code.length() )
    {
        handleAuthError( "Code was empty in the user prompt response..." );
        mAuthState = AuthState::nothing;
        return;
    }

    log( QString( "Continuing using the code '%1'..." ).arg( code ) );
    emit userGrantedPermission();

    startTokenExchangeStep( code );
}

bool OAuth2::processAccessTokenResponse( const QByteArray& data, OAuth2::AccessToken& accessToken_OUT )
{
    QJsonDocument doc = QJsonDocument::fromJson( data );
    if( !doc.isObject() )
    {
        handleAuthError( QString( "Token response did not contain the expected JSON object..." ) );
        return false;
    }

    const QString accessTokenKey    = "access_token";
    const QString tokenTypeKey      = "token_type";
    const QString expiresInKey      = "expires_in";
    const QString refreshTokenKey   = "refresh_token";

    QJsonObject object = doc.object();

    if( !object.contains( accessTokenKey ) )
    {
        handleAuthError( QString( "Token response was incomplete (%1 missing)..." ).arg( accessTokenKey ) );
        return false;
    }

    if( !object.contains( tokenTypeKey ) )
    {
        handleAuthError( QString( "Token response was incomplete (%1 missing)..." ).arg( tokenTypeKey ) );
        return false;
    }

    if( !object.contains( expiresInKey ) )
    {
        handleAuthError( QString( "Token response was incomplete (%1 missing)..." ).arg( expiresInKey ) );
        return false;
    }

    if( !object.contains( refreshTokenKey ) )
    {
        handleAuthError( QString( "Token response was incomplete (%1 missing)..." ).arg( refreshTokenKey ) );
        return false;
    }

    accessToken_OUT.accessToken     = object.value( accessTokenKey ).toString();
    accessToken_OUT.type            = object.value( tokenTypeKey ).toString();
    accessToken_OUT.lifespanSeconds = object.value( expiresInKey ).toInt();
    accessToken_OUT.refreshToken    = object.value( refreshTokenKey ).toString();
    accessToken_OUT.timestamp       = QDateTime::currentMSecsSinceEpoch();

    return true;
}

QString OAuth2::generateCodeVerifier()
{
    const QString possibleChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~";
    const int minLength = 43;
    const int maxLength = 128;

    int codeVerifierLength = generateRandomInt( minLength, maxLength );
    QString codeVerifier;
    for( int i = 0; i < codeVerifierLength; i++ )
    {
        int index = generateRandomInt( 0, possibleChars.size() - 1 );
        codeVerifier += possibleChars[ index ];
    }

    return codeVerifier;
}

QString OAuth2::generateCodeChallenge( const QString& codeVerifier )
{
    QString codeChallenge;

    if( mCodeChallengeMethod == "plain" )
    {
        codeChallenge = codeVerifier;
    }

    else if( mCodeChallengeMethod == "S256" )
    {
        // TODO
        //      The code challenge is the Base64URL (with no padding) encoded SHA256 hash of the code verifier.
    }

    return codeChallenge;
}

int OAuth2::generateRandomInt( int min, int max )
{
    return qrand() % ( max - min + 1 ) + min;
}

void OAuth2::startAuthRequestStep()
{
    mAuthState = AuthState::waiting_for_auth_request_response;
    log( "Starting auth request step..." );

    QStringList params = {

        QString( "client_id=%1" ).arg( mAuthQueryDetails.clientID ),
        QString( "redirect_uri=http://%1:%2" ).arg( mResponseAddress ).arg( mResponsePort ),
        QString( "response_type=%1" ).arg( mResponseType ),
        QString( "scope=%1" ).arg( mAuthQueryDetails.scope ),
        QString( "state=%1" ).arg( mAuthQueryDetails.state ),
        QString( "code_challenge_method=%1" ).arg( mCodeChallengeMethod ),
        QString( "code_challenge=%1" ).arg( mAuthQueryDetails.codeChallenge )

    };

    QString requestString = mAuthQueryUrls.mAuthenticationUrl + "?" + params.join( '&' );
    log( QString( "Sending auth request: %1" ).arg( requestString ) );

    auto request = QNetworkRequest( QUrl( requestString ) );
    request.setAttribute( QNetworkRequest::FollowRedirectsAttribute, true );

    auto reply = mNetworkAccessManager->get( request );
    connect( reply, QOverload<QNetworkReply::NetworkError>::of( &QNetworkReply::error ), this, &OAuth2::onReplyError );
    connect( reply, &QNetworkReply::finished, this, &OAuth2::onReplyFinished );
    connect( reply, &QNetworkReply::sslErrors, this, &OAuth2::onReplySslErrors );
}

void OAuth2::startUserPromptStep( const QString& html )
{
    mAuthState = AuthState::waiting_for_user_prompt;
    log( "Starting user prompt step..." );

    emit userPromptRequired( html );
}

void OAuth2::startTokenExchangeStep( const QString& code )
{
    mAuthState = AuthState::waiting_for_token_exchange;
    log( "Starting token exchange step..." );


    QStringList params = {

        QString( "code=%1" ).arg( code ),
        QString( "client_id=%1" ).arg( mAuthQueryDetails.clientID ),
        QString( "client_secret=%1" ).arg( mAuthQueryDetails.clientSecret ),
        QString( "grant_type=authorization_code" ),
        QString( "code_verifier=%1" ).arg( mAuthQueryDetails.codeVerifier ),
        QString( "redirect_uri=http://%1:%2" ).arg( mResponseAddress ).arg( mResponsePort )

    };

    QString requestString = mAuthQueryUrls.mTokenExchangeUrl + "?" + params.join( '&' );
    log( QString( "Sending token exchange request: %1" ).arg( requestString ) );

    auto request = QNetworkRequest( QUrl( requestString ) );
    request.setAttribute( QNetworkRequest::FollowRedirectsAttribute, true );

    auto reply = mNetworkAccessManager->post( request, QByteArray() );
    connect( reply, QOverload<QNetworkReply::NetworkError>::of( &QNetworkReply::error ), this, &OAuth2::onReplyError );
    connect( reply, &QNetworkReply::finished, this, &OAuth2::onReplyFinished );
    connect( reply, &QNetworkReply::sslErrors, this, &OAuth2::onReplySslErrors );
}
