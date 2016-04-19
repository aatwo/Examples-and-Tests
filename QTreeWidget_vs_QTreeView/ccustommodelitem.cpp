#include "ccustommodelitem.h"

CCustomModelItem::CCustomModelItem( QList<QVariant> data, CCustomModelItem* parent )
{
    mParent = parent;
    mData = data;
}

CCustomModelItem::~CCustomModelItem()
{
    for( int i = 0; i < mChildren.size(); i++ )
        delete mChildren.at( i );
}

void CCustomModelItem::appendChild(CCustomModelItem *child)
{
    mChildren.append( child );
}

CCustomModelItem* CCustomModelItem::child(int row)
{
    return mChildren.at( row );
}

int CCustomModelItem::childCount() const
{
    return mChildren.size();
}

int CCustomModelItem::columnCount() const
{
    return mData.size();
}

QVariant CCustomModelItem::data(int column) const
{
    return mData.at( column );
}

int CCustomModelItem::row() const
{
    if( mParent )
        return mParent->mChildren.indexOf( const_cast<CCustomModelItem*>( this ) );

    return 0;
}

CCustomModelItem* CCustomModelItem::parentItem()
{
    return mParent;
}
