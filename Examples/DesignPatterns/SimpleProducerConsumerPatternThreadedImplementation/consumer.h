#ifndef CONSUMER_H
#define CONSUMER_H


#include <QThread>


class Consumer : public QThread
{
    Q_OBJECT
    public:

                            Consumer( QObject* parent = nullptr );


        virtual void        run() override;


    signals:

        void                bufferFillCountChanged( int count );
        void                consumerCountChanged( int count );


}; // Consumer


#endif // CONSUMER_H
