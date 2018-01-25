#ifndef CCUSTOMSORTFILTERPROXYMODEL_H
#define CCUSTOMSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "ccommon.h"

class CCustomSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    public:
        CCustomSortFilterProxyModel( QAbstractItemModel* sourceModel, QObject* parent = nullptr );

        void SetFilters( common::FilterGroupDescription filters );


        virtual bool lessThan( const QModelIndex &left, const QModelIndex &right ) const override;
        virtual bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const;
        bool insertRows( int position, int rows, const QModelIndex &parent = QModelIndex() ) Q_DECL_OVERRIDE;


    private:

        common::FilterGroupDescription filters;
};

#endif // CCUSTOMSORTFILTERPROXYMODEL_H
