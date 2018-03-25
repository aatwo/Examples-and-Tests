#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include "pixmaplabel.h"


namespace Ui {
    class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:

        explicit            MainWindow( QWidget* parent = nullptr );
                            ~MainWindow();

    private:

        void                onAngleEditValueChanged( int value );
        void                onResetButtonPressed();
        void                onLoadButtonPressed();
        void                recomputeImage();


        Ui::MainWindow*     ui;
        PixmapLabel*        label;


}; // MainWindow


#endif // MAINWINDOW_H
