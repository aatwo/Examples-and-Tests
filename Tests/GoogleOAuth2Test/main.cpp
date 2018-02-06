#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName( "Google OAuth2 Example" );
    a.setApplicationDisplayName( "Google OAuth2 Desktop App Authentication Example" );
    a.setApplicationVersion( "0.0.1" );

    MainWindow w;
    w.show();

    return a.exec();
}
