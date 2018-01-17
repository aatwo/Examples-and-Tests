#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStatusBar>
#include <QMenuBar>
#include <QDebug>
#include <QPixmap>
#include <QImage>
#include <QLabel>
#include <QCameraInfo>


MainWindow::MainWindow( QWidget* parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    statusBar()->showMessage( "Loading..." );

    camera      = new QCamera( this );
    viewFinder  = new QCameraViewfinder( this );

    camera->setViewfinder( viewFinder );
    ui->tabWidget->addTab( viewFinder, "Camera" );

    imageCapture = new QCameraImageCapture( camera );

    camera->setCaptureMode( QCamera::CaptureStillImage );
    camera->start();

    { // Setup device combo

        ui->deviceComboBox->clear();
        QList<QCameraInfo> cameraInfoList = QCameraInfo::availableCameras();

        for( auto cameraInfo : cameraInfoList )
        {
            QString displayString = cameraInfo.description();
            ui->deviceComboBox->addItem( displayString );
        }
    }

    connect( camera, QOverload<QCamera::Error>::of(&QCamera::error), this, QOverload<QCamera::Error>::of( &MainWindow::onError ) );
    connect( camera, &QCamera::lockFailed, this, &MainWindow::onLockFailed );

    connect( imageCapture, QOverload<int, QCameraImageCapture::Error, const QString&>::of( &QCameraImageCapture::error ), this, QOverload<int, QCameraImageCapture::Error, const QString&>::of( &MainWindow::onError ) );
    connect( imageCapture, &QCameraImageCapture::imageAvailable, this, &MainWindow::onImageAvailable );
    connect( imageCapture, &QCameraImageCapture::imageCaptured, this, &MainWindow::onImageCaptured );
    connect( imageCapture, &QCameraImageCapture::imageExposed, this, &MainWindow::onImageExposed );
    connect( imageCapture, &QCameraImageCapture::imageMetadataAvailable, this, &MainWindow::onImageMetadataAvailable );
    connect( imageCapture, &QCameraImageCapture::imageSaved, this, &MainWindow::onImageSaved );
    connect( imageCapture, &QCameraImageCapture::readyForCaptureChanged, this, &MainWindow::onReadyForCaptureChanged );

    QMenu* menu = menuBar()->addMenu( "Actions" );
    menu->addAction( "Capture", this, SLOT(onCapture()) );
    connect( ui->captureButton, &QPushButton::clicked, this, &MainWindow::onCapture );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onCapture()
{
    imageCapture->capture();
}

void MainWindow::onDeviceComboIndexChanged( int index )
{
    // TODO
}

void MainWindow::onError( QCamera::Error error )
{
    Q_UNUSED( error );
    qDebug() << QString( "Camera error: %1" ).arg( camera->errorString() );
}

void MainWindow::onLockFailed()
{
    qDebug() << "Camera lock failed...";
}

void MainWindow::onError( int id, QCameraImageCapture::Error error, const QString& errorString )
{
    qDebug() << "Capture error...";
}

void MainWindow::onImageAvailable( int id, const QVideoFrame& buffer )
{
    qDebug() << "Capture image available...";

    QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat( buffer.pixelFormat() );
    QImage img( buffer.bits(), buffer.width(), buffer.height(), buffer.bytesPerLine(), imageFormat );

    QPixmap image = QPixmap::fromImage( img );
    QLabel* l = new QLabel();

    ui->tabWidget->addTab( l, QString( "%1" ).arg( id ) );

    l->setPixmap( image );
    l->show();
}

void MainWindow::onImageCaptured( int id, const QImage& preview )
{
    qDebug() << "Image captured...";
}

void MainWindow::onImageExposed( int id )
{
    qDebug() << "Capture image exposed...";
}

void MainWindow::onImageMetadataAvailable( int id, const QString& key, const QVariant& value )
{
    qDebug() << "Capture image meta data available...";
}

void MainWindow::onImageSaved( int id, const QString& fileName )
{
    qDebug() << QString( "Capture image saved (%1)..." ).arg( fileName );

    QPixmap image( fileName );
    QLabel* l = new QLabel();

    ui->tabWidget->addTab( l, QString( "%1" ).arg( id ) );

    l->setPixmap( image );
    l->show();
}

void MainWindow::onReadyForCaptureChanged( bool ready )
{
    qDebug() << QString( "Capture ready for capture changed (%1)..." ).arg( ready );
}
