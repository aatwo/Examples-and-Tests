#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:

        // TreeWidget slots
        void on_treeWidget_columnCountEdit_editingFinished();
        void on_treeWidget_sectionCountEdit_editingFinished();
        void on_treeWidget_intermittentRowsToAddEdit_editingFinished();
        void on_treeWidget_intermittentFrequencyToAddEdit_editingFinished();
        void on_treeWidget_intermittentStartButton_clicked();
        void on_treeWidget_intermittentStopButton_clicked();
        void on_treeWidget_instantRowsToAddEdit_editingFinished();
        void on_treeWidget_instantAddButton_clicked();
        void on_treeWidget_instantClearButton_clicked();
        void on_addItemTimerExpired();


        // TreeView slots
        void on_treeView_intermittentRowsToAddEdit_editingFinished();
        void on_treeView_intermittentFrequencyToAddEdit_editingFinished();
        void on_treeView_intermittentStartButton_clicked();
        void on_treeView_intermittentStopButton_clicked();
        void on_treeView_instantRowsToAddEdit_editingFinished();
        void on_treeView_instantAddButton_clicked();
        void on_treeView_instantClearButton_clicked();




    private:

        int RandomNumber( int min, int max );
        QString RandomString( int length );
        QString RandomString( int minLength, int maxLength );

        Ui::MainWindow *ui;
        QTimer treeViewTimer, treeWidgetTimer;
};

#endif // MAINWINDOW_H
