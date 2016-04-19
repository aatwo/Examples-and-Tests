#ifndef CCUSTOMMODELITEM_H
#define CCUSTOMMODELITEM_H

#include <QList>
#include <QVariant>

class CCustomModelItem
{
public:
    CCustomModelItem( QList<QVariant> data, CCustomModelItem* parent = nullptr );
    ~CCustomModelItem();

    void appendChild(CCustomModelItem *child);
    CCustomModelItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    CCustomModelItem *parentItem();


private:

    CCustomModelItem* mParent;
    QList<CCustomModelItem*> mChildren;
    QList<QVariant> mData;
};

#endif // CCUSTOMMODELITEM_H
