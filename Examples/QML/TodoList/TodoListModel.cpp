#include "TodoListModel.h"
#include "TodoListData.h"

TodoListModel::TodoListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int TodoListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if ( parent.isValid() || !todoListData )
        return 0;

    return todoListData->getItems().size();
}

QVariant TodoListModel::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() || !todoListData )
        return QVariant();

    const TodoItem item = todoListData->getItems().at( index.row() );

    switch( role )
    {
        case doneRole: return QVariant( item.done );
        case textRole: return QVariant( item.text );
    }

    return QVariant();
}

bool TodoListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( !todoListData )
        return false;

    TodoItem item = todoListData->getItems().at( index.row() );

    switch( role )
    {
        case doneRole:
        {
            item.done = value.toBool();
            break;
        }
        case textRole:
        {
            item.text = value.toString();
            break;
        }
    }

    if( todoListData->setItemAt( index.row(), item ) ) {

        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags TodoListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> TodoListModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[doneRole] = "done";
    names[textRole] = "text";
    return names;
}

TodoListData*TodoListModel::list() const
{
    return todoListData;
}

void TodoListModel::setList(TodoListData* list)
{
    beginResetModel();

    if( todoListData )
        todoListData->disconnect( this );


    delete todoListData;
    todoListData = list;

    if( todoListData ) {

        auto handlePreItemAppended = [=]() {

            const int index = todoListData->getItems().size();
            beginInsertRows( QModelIndex(), index, index );
        };

        auto handlePostItemAppended = [=]() {

            endInsertRows();
        };

        auto handlePreItemRemoved = [=]( int index ) {

            beginRemoveRows( QModelIndex(), index, index );
        };

        auto handlePostItemRemoved = [=]() {

            endRemoveRows();
        };

        connect( todoListData, &TodoListData::preItemAppended, this, handlePreItemAppended );
        connect( todoListData, &TodoListData::postItemAppended, this, handlePostItemAppended );

        connect( todoListData, &TodoListData::preItemRemoved, this, handlePreItemRemoved );
        connect( todoListData, &TodoListData::postItemRemoved, this, handlePostItemRemoved );

    }

    endResetModel();
}
