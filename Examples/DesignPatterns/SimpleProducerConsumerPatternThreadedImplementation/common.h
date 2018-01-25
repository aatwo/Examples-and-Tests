#ifndef COMMON_H
#define COMMON_H


#include <QSemaphore>


namespace common
{

    const int dataSize      = 100000;   // Amount of data to generate
    const int bufferSize    = 4096;     // Circular buffer size

    extern char buffer[ bufferSize ];

    extern QSemaphore freeBytes;
    extern QSemaphore usedBytes;


} // namespace common


#endif // COMMON_H
