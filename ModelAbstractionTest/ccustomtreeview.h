#ifndef CCUSTOMTREEVIEW_H
#define CCUSTOMTREEVIEW_H

#include <QTreeView>

class CCustomTreeView : public QTreeView
{
    Q_OBJECT
    public:
        explicit CCustomTreeView(QWidget *parent = 0);

    signals:

    public slots:
};

#endif // CCUSTOMTREEVIEW_H
