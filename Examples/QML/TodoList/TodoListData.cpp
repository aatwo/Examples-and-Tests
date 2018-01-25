#include "TodoListData.h"

TodoListData::TodoListData(QObject *parent) : QObject(parent)
{
    TodoItem itemA = { true, "Wash the car" };
    TodoItem itemB = { false, "Fix the sink" };

    todoItems.append( itemA );
    todoItems.append( itemB );
}

QVector<TodoItem> TodoListData::getItems() const
{
    return todoItems;
}

bool TodoListData::setItemAt(int index, const TodoItem& item)
{
    if( index < 0 || index >= todoItems.size() )
        return false;

    const TodoItem& oldItem = todoItems.at( index );
    if( item.done == oldItem.done && item.text == oldItem.text )
        return false;

    todoItems.replace( index, item );
    return true;
}

void TodoListData::appendItem()
{
    emit preItemAppended();

    TodoItem item;
    todoItems.append( item );

    emit postItemAppended();
}

void TodoListData::removeCompletedItems()
{
    for( int i = 0; i < todoItems.size(); i++ ) {

        if( todoItems.at( i ).done ) {

            emit preItemRemoved( i );
            todoItems.removeAt( i-- );
            emit postItemRemoved();
        }
    }
}
