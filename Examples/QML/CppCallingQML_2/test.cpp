#include "test.h"
#include <QTimer>

Test::Test(QObject *parent) : QObject(parent)
{
    mSomething = "Waiting...";

    QTimer::singleShot( 5000, this, [this](){ setSomething( "Something happened again!" ); } );
}

void Test::setSomething(QString something)
{
    mSomething = something;
    emit somethingChanged();
}

QString Test::something() const
{
    return mSomething;
}
