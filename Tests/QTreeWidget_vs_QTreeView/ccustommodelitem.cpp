#include "ccustommodelitem.h"
#include <QStringList>

CCustomModelItem::CCustomModelItem(const QVector<QVariant> &data, CCustomModelItem *parent)
{
    parentItem = parent;
    itemData = data;
}

CCustomModelItem::~CCustomModelItem()
{
    qDeleteAll(childItems);
}

CCustomModelItem *CCustomModelItem::child(int number)
{
    return childItems.value(number);
}

int CCustomModelItem::childCount() const
{
    return childItems.count();
}

int CCustomModelItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<CCustomModelItem*>(this));

    return 0;
}

int CCustomModelItem::columnCount() const
{
    return itemData.count();
}

QVariant CCustomModelItem::data(int column) const
{
    return itemData.value(column);
}

bool CCustomModelItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        CCustomModelItem *item = new CCustomModelItem(data, this);
        childItems.insert(position, item);
    }

    return true;
}

bool CCustomModelItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QVariant());

    foreach (CCustomModelItem *child, childItems)
        child->insertColumns(position, columns);

    return true;
}

CCustomModelItem *CCustomModelItem::parent()
{
    return parentItem;
}

bool CCustomModelItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}

bool CCustomModelItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.remove(position);

    foreach (CCustomModelItem *child, childItems)
        child->removeColumns(position, columns);

    return true;
}

bool CCustomModelItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}
