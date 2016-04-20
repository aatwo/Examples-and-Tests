#ifndef CFILTERGROUP_H
#define CFILTERGROUP_H

#include <QtCore>
#include <QtWidgets>
#include "cfilteritem.h"

class CFilterGroup : public QWidget
{
    Q_OBJECT
    public:

        struct FilterGroupDescription
        {
            QList< QPair<int, QString> > filterItemDescriptions; // Pairs of columns and filter-values
        };

        explicit CFilterGroup(QWidget *parent = 0);

        void Clear();
        FilterGroupDescription GetFilterGroupDescription();


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
