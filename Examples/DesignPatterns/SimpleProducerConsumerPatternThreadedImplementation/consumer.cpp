#include "consumer.h"
#include "common.h"
#include <QDebug>
#include <chrono>
#include <thread>


Consumer::Consumer( QObject* parent )
    : QThread( parent )
{
}

void Consumer::run()
{
    for( int i = 0; i < common::dataSize; i++ )
    {
        // Acquire the next used byte
        common::usedBytes.acquire();

        char data = common::buffer[ i % common::bufferSize ];
        qDebug() << QString( "Acquired byte: %1" ).arg( data );

        common::freeBytes.release();

        int bytesAvailable = common::usedBytes.available();
        qDebug() << QString( "Bytes available: %1" ).arg( bytesAvailable );


        emit bufferFillCountChanged( bytesAvailable );
        emit consumerCountChanged( i );
    }
}
