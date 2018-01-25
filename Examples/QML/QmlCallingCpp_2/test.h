#ifndef TEST_H
#define TEST_H

#include <QObject>

class Test : public QObject
{
    Q_OBJECT
    public:

        explicit Test( QObject* parent = nullptr );

        Q_INVOKABLE void testFunctionOne();
        Q_INVOKABLE void testFunctionTwo();

        Q_PROPERTY( qreal myProperty READ getMyProperty WRITE setMyProperty NOTIFY myPropertyChanged )

        Q_INVOKABLE qreal getMyProperty();
        Q_INVOKABLE void setMyProperty( qreal value );


    signals:

        void myPropertyChanged( qreal value );


    private:

        void timerTimeout();



        qreal myProperty = 1.0;

};

#endif // TEST_H
