#ifndef CCUSTOMMODEL_H
#define CCUSTOMMODEL_H

#include <QAbstractItemModel>
#include "ccustommodelitem.h"
#include <QVariant>

class CCustomModel : public QAbstractItemModel
{
    Q_OBJECT
    public:
        explicit CCustomModel(QObject *parent = 0);
        ~CCustomModel();

        virtual int	columnCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        virtual QModelIndex	index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
        virtual QModelIndex	parent(const QModelIndex &index) const override;
        virtual int	rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

    private:

        int GetRandomNumber(int min, int max);
        QString GetRandomString(int minLength, int maxLength);

        CCustomModelItem* mRoot;
};

#endif // CCUSTOMMODEL_H
