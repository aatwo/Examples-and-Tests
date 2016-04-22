#include "ccustomsortfilterproxymodel.h"
#include "ccustommodelitem.h"

CCustomSortFilterProxyModel::CCustomSortFilterProxyModel( QAbstractItemModel* sourceModel, QObject* parent )
    : QSortFilterProxyModel( parent )
{
    setSourceModel( sourceModel );
}

void CCustomSortFilterProxyModel::SetFilters(common::FilterGroupDescription filters)
{
    this->filters = filters;
    invalidateFilter();
}

bool CCustomSortFilterProxyModel::lessThan(const QModelIndex& left, const QModelIndex& right) const
{
    QVariant leftData = sourceModel()->data( left );
    QVariant rightData = sourceModel()->data( right );

    return leftData.toString() < rightData.toString();
}

bool CCustomSortFilterProxyModel::filterAcceptsRow( int sourceRow, const QModelIndex& sourceParent ) const
{
    CCustomModelItem* item = static_cast<CCustomModelItem*>( sourceParent.internalPointer() );
    if( !item || !item->parent() )
        return true; // Always accept root and sections

    bool acceptsRow = true;

    for( int i = 0; i < filters.filterItemDescriptions.size(); i++ )
    {
        int column = filters.filterItemDescriptions.at( i ).first;
        QString value = filters.filterItemDescriptions.at( i ).second;

        if( !value.length() )
            continue;

        QModelIndex dataIndex = sourceModel()->index( sourceRow, column, sourceParent );
        QString data = sourceModel()->data( dataIndex ).toString();

        if( !data.contains( value ) )
        {
            acceptsRow = false;
            break;
        }
    }

    return acceptsRow;
}

bool CCustomSortFilterProxyModel::insertRows(int position, int rows, const QModelIndex& parent)
{
    bool ret = QSortFilterProxyModel::insertRows( position, rows, parent );
    //invalidate();

    return ret;
}
