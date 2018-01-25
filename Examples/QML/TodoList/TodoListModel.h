#ifndef TODOLISTMODEL_H
#define TODOLISTMODEL_H

#include <QAbstractListModel>

class TodoListData;

class TodoListModel : public QAbstractListModel
{
    Q_OBJECT

        Q_PROPERTY( TodoListData *list READ list WRITE setList )

    public:

        enum {

            doneRole = Qt::UserRole,
            textRole
        };

        TodoListModel(QObject *parent = nullptr);

        // Basic functionality:
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;

        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

        // Editable:
        bool setData(const QModelIndex &index, const QVariant &value,
                     int role = Qt::EditRole) override;

        Qt::ItemFlags flags(const QModelIndex& index) const override;
        virtual QHash<int, QByteArray> roleNames() const override;

        TodoListData* list() const;
        void setList( TodoListData* list );

    private:

        TodoListData* todoListData = nullptr;
};

#endif // TODOLISTMODEL_H
