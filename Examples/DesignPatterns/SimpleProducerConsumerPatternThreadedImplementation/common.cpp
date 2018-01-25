#include "common.h"

char common::buffer[ common::bufferSize ];

QSemaphore common::freeBytes( common::bufferSize );
QSemaphore common::usedBytes;
