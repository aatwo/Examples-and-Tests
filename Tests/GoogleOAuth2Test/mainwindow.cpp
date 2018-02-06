#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "oauth2.h"


MainWindow::MainWindow( QWidget* parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    ui->setupUi( this );
    mAuth = new OAuth2( this );
    connect( mAuth, &OAuth2::log, this, &MainWindow::log );
    connect( ui->startButton, &QPushButton::clicked, this, &MainWindow::onStartButtonPressed );

    connect( mAuth, &OAuth2::userPromptRequired, this, &MainWindow::onUserPromptRequired );
    connect( mAuth, &OAuth2::userGrantedPermission, this, &MainWindow::onUserGrantedPermission );
    connect( mAuth, &OAuth2::authenticationError, this, &MainWindow::onAuthenticationError );
    connect( mAuth, &OAuth2::authenticated, this, &MainWindow::onAuthenticationComplete );

    // TEMP - load some default values
    ui->authenticationUrlEdit->setText( "https://accounts.google.com/o/oauth2/v2/auth" );
    ui->stateEdit->setText( "" );
    ui->loginHintEdit->setText( "" );
    ui->tokenUrlEdit->setText( "https://www.googleapis.com/oauth2/v4/token" );
    // END TEMP
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::log( QString message )
{
    ui->log->appendPlainText( "----------------------------------------" );
    ui->log->appendPlainText( message );
}

void MainWindow::onStartButtonPressed()
{
    QString authenticationUrl   = ui->authenticationUrlEdit->text();
    QString tokenExchangeUrl    = ui->tokenUrlEdit->text();
    QString clientID            = ui->clientIdEdit->text();
    QString clientSecret        = ui->clientSecretEdit->text();
    QString scope               = ui->scopeEdit->text();
    QString state               = ui->stateEdit->text();
    QString loginHint           = ui->loginHintEdit->text();

    mAuth->authenticate( authenticationUrl, tokenExchangeUrl, clientID, clientSecret, scope, state, loginHint );
}

void MainWindow::onUserPromptRequired( const QString& html )
{
    log( "MainWindow::onUserPromptRequired()..." );

    if( !mWebView )
        mWebView = new QWebEngineView();

    mWebView->setHtml( html );
    mWebView->show();
}

void MainWindow::onUserGrantedPermission()
{
    log( "MainWindow::onUserGrantedPermission()..." );
    if( mWebView )
    {
        mWebView->hide();
        mWebView->deleteLater();
    }
}

void MainWindow::onAuthenticationError()
{
    log( QString( "MainWindow::onAuthError (%1)..." ).arg( mAuth->errorString() ) );
    if( mWebView )
    {
        mWebView->hide();
        mWebView->deleteLater();
    }
}

void MainWindow::onAuthenticationComplete()
{
    auto authenticationToken = mAuth->accessToken();
    log( QString( "Authentication token:\n\taccess token: %1\n\trefresh token: %2\n\ttype: %3\n\tlifespan (seconds): %4\n\ttimestamp: %5" )
         .arg( authenticationToken.accessToken )
         .arg( authenticationToken.refreshToken )
         .arg( authenticationToken.type )
         .arg( authenticationToken.lifespanSeconds )
         .arg( authenticationToken.timestamp )
    );
}
