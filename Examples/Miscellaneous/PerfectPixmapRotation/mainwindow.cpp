#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QtWidgets>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    label = new PixmapLabel( ui->labelContainer );
    ui->labelContainerLayout->addWidget( label );

    QPixmap testPixmap( "://ShotgunMan.png" );
    label->setPixmap( testPixmap );

    connect( ui->angleEdit, QOverload<int>::of( &QSpinBox::valueChanged ), this, &MainWindow::onAngleEditValueChanged );
    connect( ui->resetButton, &QPushButton::clicked, this, &MainWindow::onResetButtonPressed );
    connect( ui->loadButton, &QPushButton::clicked, this, &MainWindow::onLoadButtonPressed );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAngleEditValueChanged( int value )
{
    label->setAngleDegrees( value );
}

void MainWindow::onResetButtonPressed()
{
    ui->angleEdit->setValue( 0 );
}

void MainWindow::onLoadButtonPressed()
{
    QString caption     = tr( "Select an image" );
    QString dir         = QStandardPaths::writableLocation( QStandardPaths::PicturesLocation );
    QString filter      = "Images (*.png *.jpg *.jpeg)";
    QString filepath    = QFileDialog::getOpenFileName( this, caption, dir, filter );

    if( !filepath.length() )
        return;

    QPixmap pixmap( filepath );
    if( pixmap.isNull() )
        return;

    label->setPixmap( pixmap );
}

void MainWindow::recomputeImage()
{
    label->setAngleDegrees( ui->angleEdit->value() );
}
