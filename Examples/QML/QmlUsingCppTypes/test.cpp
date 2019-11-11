#include "test.h"
#include <QDebug>

Test::Test(QObject *parent) : QObject(parent)
{
}

void Test::TestFunction()
{
    qDebug() << "TestFunction() called";
}

void Test::TestFunctionEnum(common::MyEnum myEnum)
{
    switch(myEnum)
    {
        case common::MyEnum::MyEnumValue1:
        {
            qDebug() << "TestFunctionEnum() called with MyEnumValue1";
            return;
        }
        case common::MyEnum::MyEnumValue2:
        {
            qDebug() << "TestFunctionEnum() called with MyEnumValue2";
            return;
        }
        case common::MyEnum::MyEnumValue3:
        {
            qDebug() << "TestFunctionEnum() called with MyEnumValue3";
            return;
        }
    }

    qDebug() << "TestFunctionEnum() called with unknown enum value";
}

void Test::TestFunctionStruct(const common::MyStruct& myStruct)
{
    qDebug() << "TestFunctionStruct() called with name: " << myStruct.getName();
}
