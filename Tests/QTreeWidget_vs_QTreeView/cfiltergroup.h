#ifndef CFILTERGROUP_H
#define CFILTERGROUP_H

#include <QtCore>
#include <QtWidgets>
#include "cfilteritem.h"
#include "ccommon.h"

class CFilterGroup : public QWidget
{
    Q_OBJECT
    public:

        explicit CFilterGroup(QWidget *parent = 0);

        void Clear();
        common::FilterGroupDescription GetFilterGroupDescription();


    signals:

        void FiltersChanged();


    public slots:

        void AddFilterItem();


    private slots:

        void FilterRemoved();


    private:

        void RebuildLayout();

        QVBoxLayout* outerLayout, *itemLayout, *spacerLayout;
        QList<CFilterItem*> filterItems;

        QTimer filterChangedTimer;
};

#endif // CFILTERGROUP_H
