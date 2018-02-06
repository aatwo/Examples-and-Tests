#ifndef OAUTH2_H
#define OAUTH2_H


#include <QObject>
#include <QTcpServer>
#include <QNetworkAccessManager>
#include <QNetworkReply>


class OAuth2 : public QObject
{
    Q_OBJECT
    public:

        struct AccessToken
        {
            QString accessToken;
            QString refreshToken;
            QString type;
            int     lifespanSeconds = 0;
            qint64  timestamp = 0;
        };

        // TODO: support the SHA256 code_challenge + code_verifier steps (https://developers.google.com/identity/protocols/OAuth2InstalledApp)
        // TODO: support saving and loading the refresh token
        // TODO: support the access token refresh query when required

                            OAuth2( QObject* parent = nullptr );

        bool                authenticate( const QString& authenticationUrl,
                                          const QString& tokenExchangeUrl,
                                          const QString& clientID,
                                          const QString& clientSecret,
                                          const QString& scope,
                                          const QString& state,
                                          const QString& loginHint = "" );

        QString             errorString();
        AccessToken         accessToken();


    signals:

        void                userPromptRequired( const QString& html );  // Emitted when a user prompt is required (you need to show this html to the user)
        void                userGrantedPermission();                    // Emitted once the user grants the required permissions
        void                authenticationError();                      // Emitted if an error occured during any authentication step
        void                authenticated();                            // Emitted when the authentication successfully finishes (call accessToken())
        void                log( QString message );


    private:

        void                onIncomingConnection();
        void                onConnectionReadyRead();
        void                onConnectionDisconnected();
        void                onConnectionError( QAbstractSocket::SocketError socketError );
        void                onReplyError( QNetworkReply::NetworkError code );
        void                onReplyFinished();
        void                onReplySslErrors( const QList<QSslError>& errors );

        void                handleAuthError( QString error );
        bool                getVariableFromString( const QString& variableName, const QString& string, QString& value_OUT );
        void                processPromptResponse( const QString& data );
        bool                processAccessTokenResponse( const QByteArray& data, AccessToken& accessToken_OUT );
        QString             generateCodeVerifier();
        QString             generateCodeChallenge( const QString& codeVerifier );
        int                 generateRandomInt( int min, int max );

        void                startAuthRequestStep();
        void                startUserPromptStep( const QString& html );
        void                startTokenExchangeStep( const QString& code );


        enum class AuthState
        {
            nothing,
            waiting_for_auth_request_response,
            waiting_for_user_prompt,
            waiting_for_token_exchange
        };

        struct AuthQueryDetails
        {
            // Data used for queries
            QString clientID;
            QString clientSecret;
            QString scope;
            QString state;
            QString loginHint;
            QString codeVerifier;
            QString codeChallenge;
        };

        struct AuthQueryUrls
        {
            // API URLS
            QString mAuthenticationUrl;
            QString mTokenExchangeUrl;
        };

        QNetworkAccessManager*  mNetworkAccessManager = nullptr;
        QTcpServer*             mResponseServer = nullptr;

        QString             mLastError;
        AuthState           mAuthState = AuthState::nothing;
        AuthQueryDetails    mAuthQueryDetails;
        AuthQueryUrls       mAuthQueryUrls;

        // Info returned by queries or generated internally
        AccessToken         mAccessToken;
        unsigned int        mResponsePort = 0; // generated randomly by the server
        const QString       mResponseAddress = "127.0.0.1";
        const QString       mResponseType = "code";
        const QString       mCodeChallengeMethod = "plain"; // Can also be "S256" for a sha256, but for now only plain is supported


}; // OAuth2


#endif // OAUTH2_H
