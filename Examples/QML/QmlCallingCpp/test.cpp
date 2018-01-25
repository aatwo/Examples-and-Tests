#include "test.h"
#include <QDebug>


Test::Test( QObject* parent )
    : QObject( parent )
{
}

void Test::testFunctionOne()
{
    qDebug() << "testFunctionOne() called...";
}

void Test::testFunctionTwo()
{
    qDebug() << "testFunctionTwo() called...";
}
