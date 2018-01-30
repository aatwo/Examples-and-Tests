#include "test.h"
#include <QTimer>

Test::Test(QObject *parent) : QObject(parent)
{
    QTimer::singleShot( 5000, this, [this](){ emit somethingHappened( "Something happened!" ); } );
}
