#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QImage>
#include <QFileDialog>
#include <QStandardPaths>
#include <QZXing.h>


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

    QImage image = imageList[ ui->imageCombo->currentIndex() ].image;
    QZXing decoder;
    decoder.setDecoder( QZXing::DecoderFormat_QR_CODE | QZXing::DecoderFormat_EAN_13 );
    QString result = decoder.decodeImage( image );

    if( result.length() )
    {
        log( QString( "Output: %1" ).arg( result ) );
        ui->output->setText( result );
    }
    else
    {
        log( "Empty output..." );
        ui->output->setText( "Empty output..." );
    }
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
