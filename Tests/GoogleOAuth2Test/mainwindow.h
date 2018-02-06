#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QWebEngineView>
#include <QPointer>


class OAuth2;


namespace Ui {
    class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:

                            MainWindow( QWidget* parent = nullptr );
                            ~MainWindow();


    private:

        void                log( QString message );
        void                onStartButtonPressed();
        void                onUserPromptRequired( const QString& html );
        void                onUserGrantedPermission();
        void                onAuthenticationError();
        void                onAuthenticationComplete();

        Ui::MainWindow*     ui;
        OAuth2*             mAuth;
        QPointer<QWebEngineView> mWebView;


}; // MainWindow


#endif // MAINWINDOW_H
