#ifndef COMMON_H
#define COMMON_H

#include <QObject>

namespace common
{
    Q_NAMESPACE

    enum class MyEnum
    {
        MyEnumValue1,
        MyEnumValue2,
        MyEnumValue3
    };
    Q_ENUMS(MyEnum);

    struct MyStruct
    {
        Q_GADGET
        Q_PROPERTY(QString Name MEMBER name READ getName WRITE setName)
        public:

            void setName(const QString& value){ name = value; }
            QString getName() const { return name; }

        private:
            QString name;
    };

    class Factory : public QObject
    {
        Q_OBJECT
        public:
            explicit Factory(QObject* parent = nullptr) : QObject(parent) {}

            Q_INVOKABLE MyStruct makeMyStruct(){ return myStruct; }

        private:

            MyStruct myStruct;
    };


} // common

#endif // COMMON_H
