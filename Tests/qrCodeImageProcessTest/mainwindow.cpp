#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QImage>
#include <QFileDialog>
#include <QStandardPaths>
#include "libdecodeqr/decodeqr.h"
#include "opencv2/imgcodecs/imgcodecs_c.h"


MainWindow::MainWindow( QWidget* parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    imageLabel = new ImageLabel( this );
    ui->imageLayout->addWidget( imageLabel );

    imageList.append( Image( "QR Code 1", ":/images/qrCode1.png" ) );
    imageList.append( Image( "QR Code 2", ":/images/qrCode2.jpg" ) );
    imageList.append( Image( "Barcode 1", ":/images/barCode1.jpg" ) );
    imageList.append( Image( "Barcode 2", ":/images/barCode2.jpg" ) );

    for( Image image : imageList )
        ui->imageCombo->addItem( image.name );

    // Set initial label image
    QImage image = imageList[ 0 ].image;
    imageLabel->setImage( image );

    connect( ui->loadButton, &QPushButton::clicked, this, &MainWindow::onLoadButtonPressed );
    connect( ui->imageCombo, QOverload<int>::of( &QComboBox::currentIndexChanged ), this, &MainWindow::onImageComboIndexChanged );
    connect( ui->processButton, &QPushButton::clicked, this, &MainWindow::onProcessButtonPressed );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onImageComboIndexChanged( int index )
{
    QImage image = imageList[ index ].image;
    imageLabel->setImage( image );
}

void MainWindow::onProcessButtonPressed()
{
    log( "Processing..." );

    QImage          qtImage     = imageList.at( ui->imageCombo->currentIndex() ).image;
    IplImage*       cvImage     = qtImageToOpenCvImage( qtImage );
    QrDecoderHandle decoder     = qr_decoder_open();
    short           stat        = qr_decoder_decode_image( decoder, cvImage );
    Q_UNUSED( stat );

    QrCodeHeader header;
    if( qr_decoder_get_header( decoder, &header ) )
    {
        char* buf = new char[ header.byte_size + 1 ];
        qr_decoder_get_body( decoder, (unsigned char*)buf, header.byte_size + 1 );

        QString output = QString( "%1" ).arg( buf );

        if( output.length() )
        {
            log( QString( "Image decoded successfully: \"%1\"" ).arg( output ) );
            ui->output->setText( output );
        }
        else
        {
            log( "Output empty..." );
            ui->output->setText( "Output empty..." );
        }
    }

    else
    {
        log( "Unable to decode image header..." );
        ui->output->setText( "Processing error (unable to decode header)..." );
    }

    qr_decoder_close( decoder );
    cvReleaseImage( &cvImage );
}

void MainWindow::onLoadButtonPressed()
{
    static QString dialogDir;
    if( !dialogDir.length() )
    {
        QStringList paths = QStandardPaths::standardLocations( QStandardPaths::PicturesLocation );
        if( paths.size() )
            dialogDir = paths[ 0 ];
    }

    QString path = QFileDialog::getOpenFileName( this, "Load image", dialogDir, "Images (*.png *.jpg *.jpeg)" );
    if( !path.length() )
        return;

    QFileInfo   info( path );
    QDir        dir( path );
    QImage      image( path );
    QString     name = info.fileName();

    dialogDir = dir.path();
    ui->imageCombo->addItem( info.fileName() );
    imageList.append( Image( name, image ) );

    ui->imageCombo->setCurrentIndex( ui->imageCombo->count() - 1 );
}

void MainWindow::log( QString m )
{
    ui->log->appendPlainText( m );
}

IplImage* MainWindow::qtImageToOpenCvImage( const QImage& qtImage )
{
    int width   = qtImage.width();
    int height  = qtImage.height();

    CvSize size;
    size.height = height;
    size.width  = width;

    IplImage* charIplImageBuffer = cvCreateImage( size, IPL_DEPTH_8U, 1 );
    char* charTemp = (char*)charIplImageBuffer->imageData;

    for( int y = 0; y < height; y++ )
    {
        for( int x = 0; x < width; x++ )
        {
            int index = y * width + x;
            charTemp[ index ] = (char)qGray( qtImage.pixel( x, y ) );
        }
    }

    return charIplImageBuffer;
}
