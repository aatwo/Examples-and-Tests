#ifndef PRODUCER_H
#define PRODUCER_H


#include <QThread>


class Producer : public QThread
{
    Q_OBJECT
    public:

                            Producer( QObject* parent = nullptr );

        virtual void        run() override;


    signals:

        void                bufferFillCountChanged( int count );
        void                producerCountChanged( int count );


}; // Producer


#endif // PRODUCER_H
