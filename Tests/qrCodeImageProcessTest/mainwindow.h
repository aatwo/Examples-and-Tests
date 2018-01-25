#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <opencv/cv.h>
#include "imagelabel.h"


namespace Ui {
    class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:

        explicit            MainWindow( QWidget* parent = 0 );
                            ~MainWindow();


    private:

        void                onImageComboIndexChanged( int index );
        void                onProcessButtonPressed();
        void                onLoadButtonPressed();
        void                log( QString m );
        IplImage*           qtImageToOpenCvImage( const QImage& qtImage );

        struct Image
        {
            Image( QString name, const QImage& image )
            {
                this->name = name;
                this->image = image;
            }

            Image( QString name, QString imagePath )
            {
                this->name = name;
                this->image = QImage( imagePath );
            }

            QString name;
            QImage image;
        };

        Ui::MainWindow*     ui;
        ImageLabel*         imageLabel;
        QList<Image>        imageList;        


}; // MainWindow


#endif // MAINWINDOW_H
