#include "ccustommodel.h"
#include "ccustommodelitem.h"
#include <QDebug>

CCustomModel::CCustomModel(CRandomStringManager* randomStringManager, const QStringList &headers, int sectionCount, QObject *parent)
    : QAbstractItemModel(parent),
      randomStringManager( randomStringManager )
{
    QVector<QVariant> rootData;
    foreach (QString header, headers)
        rootData << header;

    rootItem = new CCustomModelItem(rootData);

    // Create the sections
    for( int i = 0; i < sectionCount; i++ )
    {
        rootItem->insertChildren( i, 1, rootItem->columnCount() );
        rootItem->child( i )->setData( 0, QString( "Section %1" ).arg( i + 1 ) );
    }
}

CCustomModel::~CCustomModel()
{
    delete rootItem;
}

void CCustomModel::SetColumnCount(int columnCount)
{
    if( columnCount == rootItem->columnCount() )
        return;

    if( columnCount > rootItem->columnCount() )
    {
        for( int i = rootItem->columnCount(); i < columnCount; i++ )
        {
            insertColumn( i );
            setHeaderData( i, Qt::Horizontal, randomStringManager->RandomString() );
        }
    }
    else
    {
        for( int i = rootItem->columnCount() - 1; i >= columnCount; i-- )
        {
            removeColumn( i );
        }
    }
}

void CCustomModel::SetSectionCount( int count )
{
    if( count == rootItem->childCount() )
        return;

    if( count > rootItem->childCount() )
    {
        for( int i = rootItem->childCount(); i < count; i++ )
        {
            if( insertRow( i ) )
            {
                rootItem->child( i )->setData( 0, QString( "Section %1" ).arg( i + 1 ) );
            }
        }
    }
    else
    {
        for( int i = rootItem->childCount() - 1; i >= count; i-- )
        {
            removeRow( i );
        }
    }
}

int CCustomModel::columnCount(const QModelIndex & /* parent */) const
{
    return rootItem->columnCount();
}

QVariant CCustomModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    CCustomModelItem *item = getItem(index);

    return item->data(index.column());
}

Qt::ItemFlags CCustomModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return /*Qt::ItemIsEditable |*/ QAbstractItemModel::flags(index);
}

CCustomModelItem *CCustomModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        CCustomModelItem *item = static_cast<CCustomModelItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

QVariant CCustomModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex CCustomModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    CCustomModelItem *parentItem = getItem(parent);

    CCustomModelItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

bool CCustomModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool CCustomModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    CCustomModelItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

    return success;
}

QModelIndex CCustomModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    CCustomModelItem *childItem = getItem(index);
    CCustomModelItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool CCustomModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool CCustomModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    CCustomModelItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

int CCustomModel::rowCount(const QModelIndex &parent) const
{
    CCustomModelItem *parentItem = getItem(parent);

    return parentItem->childCount();
}

bool CCustomModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    CCustomModelItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

bool CCustomModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

//void CCustomModel::setupModelData(const QStringList &lines, CCustomModelItem *parent)
//{
//    QList<CCustomModelItem*> parents;
//    QList<int> indentations;
//    parents << parent;
//    indentations << 0;

//    int number = 0;

//    while (number < lines.count()) {
//        int position = 0;
//        while (position < lines[number].length()) {
//            if (lines[number].at(position) != ' ')
//                break;
//            ++position;
//        }

//        QString lineData = lines[number].mid(position).trimmed();

//        if (!lineData.isEmpty()) {
//            // Read the column data from the rest of the line.
//            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
//            QVector<QVariant> columnData;
//            for (int column = 0; column < columnStrings.count(); ++column)
//                columnData << columnStrings[column];

//            if (position > indentations.last()) {
//                // The last child of the current parent is now the new parent
//                // unless the current parent has no children.

//                if (parents.last()->childCount() > 0) {
//                    parents << parents.last()->child(parents.last()->childCount()-1);
//                    indentations << position;
//                }
//            } else {
//                while (position < indentations.last() && parents.count() > 0) {
//                    parents.pop_back();
//                    indentations.pop_back();
//                }
//            }

//            // Append a new item to the current parent's list of children.
//            CCustomModelItem *parent = parents.last();
//            parent->insertChildren(parent->childCount(), 1, rootItem->columnCount());
//            for (int column = 0; column < columnData.size(); ++column)
//                parent->child(parent->childCount() - 1)->setData(column, columnData[column]);
//        }

//        ++number;
//    }
//}
