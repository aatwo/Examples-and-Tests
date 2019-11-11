#ifndef TEST_H
#define TEST_H

#include <QObject>
#include "common.h"

class Test : public QObject
{
    Q_OBJECT
    public:
        explicit Test(QObject *parent = nullptr);

        Q_INVOKABLE void TestFunction();
        Q_INVOKABLE void TestFunctionEnum(common::MyEnum myEnum);
        Q_INVOKABLE void TestFunctionStruct(const common::MyStruct& myStruct);
};

#endif // TEST_H
