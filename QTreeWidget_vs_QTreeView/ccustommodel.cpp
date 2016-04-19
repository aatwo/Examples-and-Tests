#include "ccustommodel.h"

CCustomModel::CCustomModel(QObject *parent) : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << "Column A" << "Column B" << "Column C" << "Column D" << "Column E";
    mRoot = new CCustomModelItem( rootData );

    QList<QVariant> rowOneData, rowTwoData, rowThreeData;

    rowOneData << "Row One:" << "" << "" << "" << "";
    rowTwoData << "Row Two:" << "" << "" << "" << "";
    rowThreeData << "Row Three:" << "" << "" << "" << "";

    CCustomModelItem* dataSetOneItem    = new CCustomModelItem( rowOneData, mRoot );
    CCustomModelItem* dataSetTwoItem    = new CCustomModelItem( rowTwoData, mRoot );
    CCustomModelItem* dataSetThreeItem  = new CCustomModelItem( rowThreeData, mRoot );

    for( int i = 0; i < 10; i++ )
    {
        QList<QVariant> childRowData;

        for( int column = 0; column < 5; column++ )
            childRowData.append( GetRandomString( 10, 10 ) );

        CCustomModelItem* childItem = new CCustomModelItem( childRowData, dataSetOneItem );
        dataSetOneItem->appendChild( childItem );
    }

    for( int i = 0; i < 10; i++ )
    {
        QList<QVariant> childRowData;

        for( int column = 0; column < 5; column++ )
            childRowData.append( GetRandomString( 10, 10 ) );

        CCustomModelItem* childItem = new CCustomModelItem( childRowData, dataSetTwoItem );
        dataSetTwoItem->appendChild( childItem );
    }

    for( int i = 0; i < 10; i++ )
    {
        QList<QVariant> childRowData;

        for( int column = 0; column < 5; column++ )
            childRowData.append( GetRandomString( 10, 10 ) );

        CCustomModelItem* childItem = new CCustomModelItem( childRowData, dataSetThreeItem );
        dataSetThreeItem->appendChild( childItem );
    }

    mRoot->appendChild( dataSetOneItem );
    mRoot->appendChild( dataSetTwoItem );
    mRoot->appendChild( dataSetThreeItem );
}

CCustomModel::~CCustomModel()
{
    delete mRoot;
}

int CCustomModel::columnCount(const QModelIndex &parent) const
{
    if( parent.isValid() )
        return static_cast<CCustomModelItem*>( parent.internalPointer() )->columnCount();
    else
        return mRoot->columnCount();
}

QVariant CCustomModel::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() )
        return QVariant();

    if( role != Qt::DisplayRole )
        return QVariant();

    CCustomModelItem* item = static_cast<CCustomModelItem*>( index.internalPointer() );

    return item->data( index.column() );
}

QModelIndex CCustomModel::index(int row, int column, const QModelIndex &parent) const
{
    if( !hasIndex( row, column, parent ) )
        return QModelIndex();

    CCustomModelItem* parentItem;

    if( !parent.isValid() )
        parentItem = mRoot;
    else
        parentItem = static_cast<CCustomModelItem*>( parent.internalPointer() );

    CCustomModelItem* childItem = parentItem->child( row );
    if( childItem )
        return createIndex( row, column, childItem );
    else
        return QModelIndex();
}

QModelIndex CCustomModel::parent(const QModelIndex &index) const
{
    if( !index.isValid() )
        return QModelIndex();

    CCustomModelItem* childItem = static_cast<CCustomModelItem*>( index.internalPointer() );
    CCustomModelItem* parentItem = childItem->parentItem();

    if( parentItem == mRoot )
        return QModelIndex();

    return createIndex( parentItem->row(), 0, parentItem );
}

int CCustomModel::rowCount(const QModelIndex &parent) const
{
    CCustomModelItem* parentItem;
    if( parent.column() > 0 )
        return 0;

    if( !parent.isValid() )
        parentItem = mRoot;
    else
        parentItem = static_cast<CCustomModelItem*>( parent.internalPointer() );

    return parentItem->childCount();
}

Qt::ItemFlags CCustomModel::flags(const QModelIndex &index) const
{
    if( !index.isValid() )
        return 0;

    return QAbstractItemModel::flags( index );
}

int CCustomModel::GetRandomNumber(int min, int max)
{
    return qrand() % (max - min + 1) + min;
}

QString CCustomModel::GetRandomString(int minLength, int maxLength)
{
    QString ret;
    int length = GetRandomNumber( minLength, maxLength );

    for( int i = 0; i < length; i++ )
    {
        char randomChar = static_cast<char>( GetRandomNumber( 33, 126 ) );
        ret.append( randomChar );
    }

    return ret;
}

