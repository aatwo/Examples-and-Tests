#include "cfiltergroup.h"

CFilterGroup::CFilterGroup(QWidget *parent) : QWidget(parent)
{
    outerLayout = nullptr;
    itemLayout = nullptr;
    spacerLayout = nullptr;

    filterChangedTimer.setSingleShot( true );


    RebuildLayout();
}

void CFilterGroup::Clear()
{
    delete outerLayout;

    outerLayout = nullptr;
    itemLayout = nullptr;
    spacerLayout = nullptr;

    for( CFilterItem* item : filterItems )
        delete item;

    filterItems.clear();

    emit FiltersChanged();
}

common::FilterGroupDescription CFilterGroup::GetFilterGroupDescription()
{
    common::FilterGroupDescription ret;

    for( CFilterItem* item : filterItems )
    {
        QString value = item->GetValue();
        if( !value.length() )
            continue;

        QPair<int, QString> filterData( item->GetColumn(), value );
        ret.filterItemDescriptions.append( filterData );
    }

    return ret;
}

void CFilterGroup::AddFilterItem()
{
    if( !outerLayout )
        RebuildLayout();

    CFilterItem* item = new CFilterItem( this );
    filterItems.append( item );

    connect( item, &CFilterItem::FilterRemoved, this, &CFilterGroup::FilterRemoved );
    connect( item, &CFilterItem::FilterChanged, this, &CFilterGroup::FiltersChanged );

    itemLayout->addWidget( item );
}

void CFilterGroup::FilterRemoved()
{
    CFilterItem* item = qobject_cast<CFilterItem*>( QObject::sender() );
    if( !item )
        return;

    filterItems.removeAll( item );
    item->deleteLater();

    RebuildLayout();

    emit FiltersChanged();
}

void CFilterGroup::RebuildLayout()
{
    delete outerLayout;

    outerLayout = new QVBoxLayout( this );
    itemLayout  = new QVBoxLayout;
    spacerLayout = new QVBoxLayout;

    for( CFilterItem* item : filterItems )
    {
        itemLayout->addWidget( item );
    }

    outerLayout->addLayout( itemLayout );
    outerLayout->addLayout( spacerLayout );

    spacerLayout->addItem( new QSpacerItem( 10, 10, QSizePolicy::Fixed, QSizePolicy::MinimumExpanding ) );
}
