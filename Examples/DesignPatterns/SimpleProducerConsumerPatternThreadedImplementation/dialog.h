#ifndef DIALOG_H
#define DIALOG_H


#include <QtWidgets>
#include "producer.h"
#include "consumer.h"


class Dialog : public QWidget
{
    Q_OBJECT
    public:

                            Dialog( QWidget* parent = nullptr );
                            ~Dialog();


    private slots:

        void                onStartButtonPressed();
        void                onQuitButtonPressed();


    private:

        QGridLayout*        layout              = nullptr;
        QProgressBar*       producerProgressBar = nullptr;
        QProgressBar*       consumerProgressBar = nullptr;
        QProgressBar*       bufferProgressBar   = nullptr;
        QPushButton*        startButton         = nullptr;
        QPushButton*        quitButton          = nullptr;

        Producer*           producer = nullptr;
        Consumer*           consumer = nullptr;

}; // Dialog


#endif // DIALOG_H
