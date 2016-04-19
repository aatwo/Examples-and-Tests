#ifndef CCUSTOMMODELITEM_H
#define CCUSTOMMODELITEM_H

#include <QList>
#include <QVariant>
#include <QVector>

class CCustomModelItem
{
public:
    explicit CCustomModelItem(const QVector<QVariant> &data, CCustomModelItem *parent = 0);
    ~CCustomModelItem();

    CCustomModelItem *child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);
    CCustomModelItem *parent();
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber() const;
    bool setData(int column, const QVariant &value);

private:
    QList<CCustomModelItem*> childItems;
    QVector<QVariant> itemData;
    CCustomModelItem *parentItem;
};

#endif // CCUSTOMMODELITEM_H
