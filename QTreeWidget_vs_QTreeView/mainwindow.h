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
        void on_treeWidget_instantAddButton_clicked();
        void on_treeWidget_resizeColumnsButton_clicked();
        void on_treeWidget_instantClearButton_clicked();
        void on_treeWidget_addItemTimerExpired();
        void on_treeWidget_expandAllButton_clicked();
        void on_treeWidget_collapseAllButton_clicked();


        // TreeView slots
        void on_treeView_intermittentRowsToAddEdit_editingFinished();
        void on_treeView_intermittentFrequencyToAddEdit_editingFinished();
        void on_treeView_intermittentStartButton_clicked();
        void on_treeView_intermittentStopButton_clicked();
        void on_treeView_instantRowsToAddEdit_editingFinished();
        void on_treeView_instantAddButton_clicked();
        void on_treeView_instantClearButton_clicked();


        // Random string slots
        void on_randomStringGenerateButton_clicked();
        void on_randomStringClearButton_clicked();


        void on_clearLogButton_clicked();

    private:

        // Random functions
        int RandomNumber( int min, int max );
        QString RandomString( int length );
        QString RandomString();
        QStringList RandomStringList( int stringCount );

        // Other functions
        void Log( QString message );
        void SetStatus( QString status );
        void ClearStatus();
        void UpdateRandomStringCountLabel();
        void UpdateTreeWidgetRowCountLabel();


        Ui::MainWindow *ui;
        QTimer treeViewTimer, treeWidgetTimer;
        int treeWidgetRowCount;

        QQueue<QString> randomStringQueue;
};

#endif // MAINWINDOW_H
