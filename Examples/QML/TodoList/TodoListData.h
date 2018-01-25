#ifndef TODOLISTDATA_H
#define TODOLISTDATA_H

#include <QObject>
#include <QVector>


struct TodoItem {

    TodoItem() {}
    TodoItem( bool done, QString text ) : done( done ), text( text ) {}
    bool done = false;
    QString text = "";
};


class TodoListData : public QObject
{
    Q_OBJECT
    public:

        explicit TodoListData(QObject *parent = nullptr);

        QVector<TodoItem> getItems() const;
        bool setItemAt( int index, const TodoItem& item );


    signals:

        void preItemAppended();
        void postItemAppended();

        void preItemRemoved( int index );
        void postItemRemoved();


    public slots:

        void appendItem();
        void removeCompletedItems();


    private:

        QVector<TodoItem> todoItems;

};

#endif // TODOLISTDATA_H
