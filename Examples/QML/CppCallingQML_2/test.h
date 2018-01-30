#ifndef TEST_H
#define TEST_H

#include <QObject>

class Test : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString something READ something WRITE setSomething NOTIFY somethingChanged)

    public:

        explicit Test(QObject *parent = nullptr);

        void setSomething( QString something );
        QString something() const;


    signals:

        void somethingChanged();


    private:

        QString mSomething;


};

#endif // TEST_H
