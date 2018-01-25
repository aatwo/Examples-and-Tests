#include "producer.h"
#include "common.h"
#include <QDateTime>


Producer::Producer( QObject* parent )
    : QThread( parent )
{
}

void Producer::run()
{
    qsrand( QDateTime::currentMSecsSinceEpoch() );

    for( int i = 0; i < common::dataSize; i++ )
    {
        // Acquire one free byte
        common::freeBytes.acquire();

        // Determine the index into the circular buffer for the current piece of data
        // For example when i is 4, bufferSize is 8192 and so 4 % 8192 = 4. When i is
        // 8200, 8200 % 8192 = 8 and thus it wraps back round. This is a neat trick to
        // use for circular buffer indexes

        int     index       = i % common::bufferSize;
        char    randomChar  = "ACGT"[ (int)qrand() % 4 ];

        common::buffer[ index ] = randomChar;

        // Free one free byte
        common::usedBytes.release();

        if( i % 20 == 0 )
        {
            int bytesAvailable = common::usedBytes.available();
            emit bufferFillCountChanged( bytesAvailable );
        }

        emit producerCountChanged( i );
    }
}
