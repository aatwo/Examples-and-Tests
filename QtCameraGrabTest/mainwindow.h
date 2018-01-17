#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>


namespace Ui {
    class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:

        explicit            MainWindow( QWidget* parent = nullptr );
                            ~MainWindow();


    private slots:

        void                onCapture();


    private:

        void                onDeviceComboIndexChanged( int index );

        void                onError( QCamera::Error error );
        void                onLockFailed();

        void                onError( int id, QCameraImageCapture::Error error, const QString& errorString );
        void                onImageAvailable( int id, const QVideoFrame& buffer );
        void                onImageCaptured( int id, const QImage& preview );
        void                onImageExposed( int id );
        void                onImageMetadataAvailable( int id, const QString& key, const QVariant& value );
        void                onImageSaved( int id, const QString& fileName );
        void                onReadyForCaptureChanged( bool ready );


        Ui::MainWindow*     ui              = nullptr;
        QCamera*            camera          = nullptr;
        QCameraViewfinder*  viewFinder      = nullptr;
        QCameraImageCapture* imageCapture   = nullptr;


}; // MainWindow


#endif // MAINWINDOW_H
