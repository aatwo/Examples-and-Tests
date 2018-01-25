#include "dialog.h"
#include "common.h"


Dialog::Dialog( QWidget* parent )
    : QWidget( parent )
{
    layout = new QGridLayout( this );

    auto producedLabel          = new QLabel( "Produced: ", this );
    auto producedPercentLabel   = new QLabel( "0%", this );
    producerProgressBar         = new QProgressBar( this );

    auto consumedLabel          = new QLabel( "Produced: ", this );
    auto consumedPercentLabel   = new QLabel( "0%", this );
    consumerProgressBar         = new QProgressBar( this );

    auto bufferLabel            = new QLabel( "CurrentBufferLevel: ", this );
    auto bufferPercentLabel     = new QLabel( "0%", this );
    bufferProgressBar           = new QProgressBar( this );

    startButton = new QPushButton( "Start", this );
    quitButton  = new QPushButton( "Quit", this );

    producerProgressBar->setRange( 0, common::dataSize );
    consumerProgressBar->setRange( 0, common::dataSize );
    bufferProgressBar->setRange( 0, common::bufferSize );

    layout->addWidget( producedLabel,           0, 0, 1, 1 );
    layout->addWidget( producerProgressBar,     0, 1, 1, 1 );
    layout->addWidget( producedPercentLabel,    0, 2, 1, 1 );

    layout->addWidget( consumedLabel,           1, 0, 1, 1 );
    layout->addWidget( consumerProgressBar,     1, 1, 1, 1 );
    layout->addWidget( consumedPercentLabel,    1, 2, 1, 1 );

    layout->addWidget( bufferLabel,             2, 0, 1, 1 );
    layout->addWidget( bufferProgressBar,       2, 1, 1, 1 );
    layout->addWidget( bufferPercentLabel,      2, 2, 1, 1 );

    auto buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget( startButton );
    buttonLayout->addItem( new QSpacerItem( 10, 10, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed ) );
    buttonLayout->addWidget( quitButton );
    layout->addLayout( buttonLayout, 3, 0, 1, -1 );

    producer = new Producer( this );
    consumer = new Consumer( this );

    connect( consumer, &Consumer::bufferFillCountChanged, bufferProgressBar, &QProgressBar::setValue );
    //connect( producer, &Producer::bufferFillCountChanged, bufferProgressBar, &QProgressBar::setValue );

    connect( consumer, &Consumer::consumerCountChanged, consumerProgressBar, &QProgressBar::setValue );
    connect( producer, &Producer::producerCountChanged, producerProgressBar, &QProgressBar::setValue );

    connect( startButton, &QPushButton::clicked, this, &Dialog::onStartButtonPressed );
    connect( quitButton, &QPushButton::clicked, this, &Dialog::onQuitButtonPressed );
}

Dialog::~Dialog()
{
}

void Dialog::onStartButtonPressed()
{
    startButton->setEnabled( false );
    producer->start();
    consumer->start();
}

void Dialog::onQuitButtonPressed()
{
    // TODO
}
