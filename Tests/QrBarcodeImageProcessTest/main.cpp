#include "mainwindow.h"
#include <QApplication>
#include <cstdint>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setOrganizationName( "aatwo" );
    a.setApplicationName( "QR / Barcode Reader" );
    a.setApplicationDisplayName( "Qr / Barcode Reader" );
    a.setApplicationVersion( "0.0.1" );

    MainWindow w;
    w.show();

    return a.exec();
}
