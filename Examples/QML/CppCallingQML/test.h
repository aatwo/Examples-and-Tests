#ifndef TEST_H
#define TEST_H

#include <QObject>

class Test : public QObject
{
    Q_OBJECT
    public:

        explicit Test(QObject *parent = nullptr);

    signals:

        void somethingHappened( QString something ); // Note: param name is important


};

#endif // TEST_H
