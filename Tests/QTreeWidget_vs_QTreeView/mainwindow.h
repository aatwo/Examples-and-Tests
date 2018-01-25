#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "ccustommodel.h"
#include "ccustommodelitem.h"
#include "crandomstringmanager.h"
#include "cfiltergroup.h"
#include "ccustomsortfilterproxymodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    friend class AutoLogger;
    Q_OBJECT
    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:

        // TreeWidget slots
        void on_treeWidget_columnCountEdit_editingFinished();
        void on_treeWidget_sectionCountEdit_editingFinished();
        void on_treeWidget_intermittentFrequencyToAddEdit_editingFinished();
        void on_treeWidget_intermittentStartButton_clicked();
        void on_treeWidget_intermittentStopButton_clicked();
        void on_treeWidget_instantAddButton_clicked();
        void on_treeWidget_resizeColumnsButton_clicked();
        void on_treeWidget_instantClearButton_clicked();
        void on_treeWidget_addItemTimerExpired();
        void on_treeWidget_expandAllButton_clicked();
        void on_treeWidget_collapseAllButton_clicked();
        void on_treeWidget_addFilterButton_clicked();
        void on_treeWidget_clearFiltersButton_clicked();
        void on_treeWidget_FiltersChanged();
        void on_treeWidget_allowSortingEdit_toggled(bool checked);


        // TreeView slots
        void on_treeView_intermittentFrequencyToAddEdit_editingFinished();
        void on_treeView_intermittentStartButton_clicked();
        void on_treeView_intermittentStopButton_clicked();
        void on_treeView_instantAddButton_clicked();
        void on_treeView_instantClearButton_clicked();
        void on_treeView_columnCountEdit_editingFinished();
        void on_treeView_resizeColumnsButton_clicked();
        void on_treeView_addItemTimerExpired();
        void on_treeView_expandAllButton_clicked();
        void on_treeView_collapseAllButton_clicked();
        void on_treeView_sctionCountEdit_editingFinished();
        void on_treeView_clearFiltersButton_clicked();
        void on_treeView_addFilterButton_clicked();
        void on_treeView_FiltersChanged();
        void on_treeView_sortingEnabledEdit_toggled(bool checked);


        // Random string slots
        void on_randomStringGenerateButton_clicked();
        void on_randomStringClearButton_clicked();
        void on_clearLogButton_clicked();
        void on_randomStringMinLength_editingFinished();
        void on_randomStringMaxLength_editingFinished();
        void on_randomStringPrependWithStringEdit_toggled(bool);
        void on_randomStringInjectWithStringEdit_toggled(bool);
        void on_randomStringAppendWithStringEdit_toggled(bool);
        void on_randomStringPrependWithStringValueEdit_textChanged(const QString &);
        void on_randomStringInjectWithStringValueEdit_textChanged(const QString &);
        void on_randomStringAppendWithStringValueEdit_textChanged(const QString &);



    private:

        // Other functions
        void Log( QString message );
        void LogNewLine( QString message );
        void SetStatus( QString status );
        void ClearStatus();
        void UpdateRandomStringCountLabel();
        void UpdateTreeWidgetRowCountLabel();
        void UpdateTreeViewRowCountLabel();
        bool IsTreeWidgetItemVisible( common::FilterGroupDescription& filterDescriptions_in, QTreeWidgetItem* item );
        void UpdateVisibleTreeWidgetRowCount();


        class AutoLogger
        {
            // Don't judge me, I'm lazy :D
            public:
                AutoLogger( MainWindow* window, QString message )
                {
                    this->window = window;
                    timer.start();

                    window->LogNewLine( "<font color=\"Black\">" + message + "...</font>" );
                    window->SetStatus( message + "..." );
                }

                ~AutoLogger()
                {
                    window->ClearStatus();

                    qint64 durationMs = timer.elapsed();
                    double seconds = static_cast<double>( durationMs ) / 1000.0;

                    QString timeColour = "Green";
                    if( durationMs > 1000 )
                        timeColour = "Red";
                    else if( durationMs > 300 )
                        timeColour = "Orange";

                    window->Log( QString( " <font color=\"%3\">Finished in %1 ms / %2 seconds</font>" ).arg( timer.elapsed() ).arg( seconds ).arg( timeColour ) );
                }

            private:
                MainWindow* window;
                QElapsedTimer timer;
        };


        Ui::MainWindow *ui;
        QTimer treeViewTimer, treeWidgetTimer;
        QElapsedTimer treeViewElapsedTimer, treeWidgetElapsedTimer;
        int treeWidgetRowCount, treeViewRowCount;
        int treeWidgetVisibleRowCount;
        CFilterGroup* treeWidgetFilterGroup, *treeViewFilterGroup;
        CCustomModel* treeViewModel;
        CCustomSortFilterProxyModel* treeViewProxyModel;
        CRandomStringManager* randomStringManager;
};

#endif // MAINWINDOW_H
