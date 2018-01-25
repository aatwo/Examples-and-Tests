#ifndef CFILTERITEM_H
#define CFILTERITEM_H

#include <QtCore>
#include <QtWidgets>

class CFilterItem : public QWidget
{
    Q_OBJECT
    public:
        explicit CFilterItem(QWidget *parent = 0);

        int GetColumn();
        QString GetValue();

    signals:

        void FilterChanged();
        void FilterRemoved();

    private:

        QLineEdit* valueEdit;
        QSpinBox* columnEdit;
};

#endif // CFILTERITEM_H
