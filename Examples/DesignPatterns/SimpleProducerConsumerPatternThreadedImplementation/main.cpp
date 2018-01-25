#include <QApplication>
#include "common.h"
#include <QDebug>
#include "dialog.h"


int main( int argc, char* argv[] )
{
    QApplication a(argc, argv);

    Dialog d;
    d.resize( 380, 180 );
    d.show();

    return a.exec();
}
