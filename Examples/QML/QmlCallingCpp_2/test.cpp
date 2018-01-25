#include "test.h"
#include <QDebug>
#include <QTimer>


Test::Test( QObject* parent )
    : QObject( parent )
{
    QTimer* t = new QTimer( this );
    t->start( 2000 );
    connect( t, &QTimer::timeout, this, &Test::timerTimeout );
}

void Test::testFunctionOne()
{
    qDebug() << "testFunctionOne() called...";
}

void Test::testFunctionTwo()
{
    qDebug() << "testFunctionTwo() called...";
}

qreal Test::getMyProperty()
{
    return myProperty;
}

void Test::setMyProperty( qreal value )
{
    myProperty = value;
    emit myPropertyChanged( value );
}

void Test::timerTimeout()
{
    static int tickCount = 0;
    qDebug() << QString( "Timer tick (%1)..." ).arg( tickCount + 1 );

    if( ( tickCount % 2 ) > 0 )
        setMyProperty( 0.25 );
    else
        setMyProperty( 0.75 );

    tickCount++;
}
