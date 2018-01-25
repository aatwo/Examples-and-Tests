#include <QApplication>
#include "mainwindow.h"
#include <Box2D/Box2D.h>

int main(int argc, char *argv[])
{
    srand(QTime::currentTime().msec());
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
