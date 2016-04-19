#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    randomStringManager( new CRandomStringManager )
{
    ui->setupUi(this);

    { // Setup TreeWidget

        treeWidgetRowCount = 0;
        qsrand( QDateTime::currentMSecsSinceEpoch() );

        on_treeWidget_columnCountEdit_editingFinished();
        on_treeWidget_sectionCountEdit_editingFinished();
        on_treeWidget_intermittentFrequencyToAddEdit_editingFinished();

        connect( &treeWidgetTimer, &QTimer::timeout, this, &MainWindow::on_treeWidget_addItemTimerExpired );
    }

    { // Setup TreeView

        QStringList headers;
        for( int i = 0; i < ui->treeView_columnCountEdit->value(); i++ )
            headers << randomStringManager->RandomString();

        treeViewModel = new CCustomModel( randomStringManager, headers, ui->treeView_sctionCountEdit->value(), ui->treeView );
        ui->treeView->setModel( treeViewModel );
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_treeWidget_columnCountEdit_editingFinished()
{
    int newColumnCount = ui->treeWidget_columnCountEdit->value();
    if( newColumnCount == ui->treeWidget->columnCount() )
        return;

    QStringList headerLabels;
    ui->treeWidget->setColumnCount( newColumnCount );
    for( int i = 0; i < newColumnCount; i++ )
    {
        headerLabels.append( randomStringManager->RandomString() );
    }
    ui->treeWidget->setHeaderLabels( headerLabels );
}

void MainWindow::on_treeWidget_sectionCountEdit_editingFinished()
{
    int currentSectionCount = ui->treeWidget->topLevelItemCount();
    int newSectionCount = ui->treeWidget_sectionCountEdit->value();

    if( currentSectionCount == newSectionCount )
        return;

    if( newSectionCount < currentSectionCount )
    {
        for( int i = currentSectionCount - 1; i >= newSectionCount; i-- )
        {
            QTreeWidgetItem* item = ui->treeWidget->takeTopLevelItem( i );
            for( int j = item->childCount() - 1; j >= 0; j-- )
            {
                QTreeWidgetItem* childItem = item->takeChild( j );
                delete childItem;
            }
            delete item;
        }
    }
    else
    {
        for( int i = currentSectionCount - 1; i < newSectionCount - 1; i++ )
        {
            QStringList sectionTitle = QStringList() << QString( "Section %1" ).arg( i + 2 );
            QTreeWidgetItem* item = new QTreeWidgetItem( sectionTitle );
            ui->treeWidget->addTopLevelItem( item );
        }
    }

    ui->treeWidget_instantSectionToAddToEdit->setMaximum( newSectionCount );
    ui->treeWidget_intermittentSectionToAddToValueEdit->setMaximum( newSectionCount );
}

void MainWindow::on_treeWidget_intermittentRowsToAddEdit_editingFinished()
{

}

void MainWindow::on_treeWidget_intermittentFrequencyToAddEdit_editingFinished()
{
    treeWidgetTimer.setInterval( ui->treeWidget_intermittentFrequencyToAddEdit->value() );
}

void MainWindow::on_treeWidget_intermittentStartButton_clicked()
{
    treeWidgetTimer.start();
}

void MainWindow::on_treeWidget_intermittentStopButton_clicked()
{
    treeWidgetTimer.stop();
}

void MainWindow::on_treeWidget_instantAddButton_clicked()
{
    QElapsedTimer timer;
    timer.start();

    int rowCount = ui->treeWidget_instantRowsToAddEdit->value();
    int columnCount = ui->treeWidget->columnCount();
    int randomStringGenerateCount = rowCount * columnCount;

    if( randomStringGenerateCount > randomStringManager->RandomStringCount() )
    {
        Log( QString( "Warning: string queue too small. %1 random strings will be generated..." ).arg( randomStringGenerateCount - randomStringManager->RandomStringCount() ) );
    }

    SetStatus( QString ( "Adding %1 rows of data..." ).arg( randomStringGenerateCount ) );

    int sectionValue = ui->treeWidget_instantSectionToAddToEdit->value();
    if( !sectionValue )
        sectionValue = randomStringManager->RandomNumber( 1, ui->treeWidget->topLevelItemCount() );

    sectionValue = sectionValue - 1;
    QTreeWidgetItem* sectionItem = ui->treeWidget->topLevelItem( sectionValue );

    for( int i = 0; i < rowCount; i++ )
    {
        QStringList rowData = randomStringManager->RandomStringList( ui->treeWidget->columnCount() );
        QTreeWidgetItem* item = new QTreeWidgetItem( rowData );

        sectionItem->addChild( item );
    }

    qint64 msElapsed = timer.elapsed();
    Log( QString( "Finished adding data to the tree widget %1 ms" ).arg( msElapsed ) );

    treeWidgetRowCount += rowCount;
    UpdateTreeWidgetRowCountLabel();
    UpdateRandomStringCountLabel();
    ClearStatus();
}

void MainWindow::on_treeWidget_resizeColumnsButton_clicked()
{
    for( int i = 0; i < ui->treeWidget->columnCount(); i++ )
        ui->treeWidget->resizeColumnToContents( i );
}

void MainWindow::on_treeWidget_instantClearButton_clicked()
{
    for( int i = 0; i < ui->treeWidget->topLevelItemCount(); i++ )
    {
        QTreeWidgetItem* item = ui->treeWidget->topLevelItem( i );
        for( int j = item->childCount() - 1; j >= 0; j-- )
        {
            QTreeWidgetItem* childItem = item->takeChild( j );
            delete childItem;
        }
    }

    treeWidgetRowCount = 0;
    UpdateTreeWidgetRowCountLabel();
}

void MainWindow::on_treeWidget_addItemTimerExpired()
{
    QElapsedTimer timer;
    timer.start();

    int rowCount = ui->treeWidget_intermittentRowsToAddEdit->value();
    int columnCount = ui->treeWidget->columnCount();
    int randomStringGenerateCount = rowCount * columnCount;

    if( randomStringGenerateCount > randomStringManager->RandomStringCount() )
    {
        Log( QString( "Warning: string queue too small. %1 random strings will be generated..." ).arg( randomStringGenerateCount - randomStringManager->RandomStringCount() ) );
    }

    SetStatus( QString ( "Adding %1 rows of data..." ).arg( randomStringGenerateCount ) );

    int sectionValue = ui->treeWidget_intermittentSectionToAddToValueEdit->value();
    if( !sectionValue )
        sectionValue = randomStringManager->RandomNumber( 1, ui->treeWidget->topLevelItemCount() );

    sectionValue = sectionValue - 1;
    QTreeWidgetItem* sectionItem = ui->treeWidget->topLevelItem( sectionValue );

    for( int i = 0; i < rowCount; i++ )
    {
        QStringList rowData = randomStringManager->RandomStringList( ui->treeWidget->columnCount() );
        QTreeWidgetItem* item = new QTreeWidgetItem( rowData );

        sectionItem->addChild( item );
    }

    qint64 msElapsed = timer.elapsed();
    Log( QString( "Finished adding data to the tree widget %1 ms" ).arg( msElapsed ) );

    treeWidgetRowCount += rowCount;
    UpdateTreeWidgetRowCountLabel();
    UpdateRandomStringCountLabel();
    ClearStatus();
}

void MainWindow::on_treeWidget_expandAllButton_clicked()
{
    ui->treeWidget->expandAll();
}

void MainWindow::on_treeWidget_collapseAllButton_clicked()
{
    ui->treeWidget->collapseAll();
}

void MainWindow::on_treeView_intermittentRowsToAddEdit_editingFinished()
{

}

void MainWindow::on_treeView_intermittentFrequencyToAddEdit_editingFinished()
{

}

void MainWindow::on_treeView_intermittentStartButton_clicked()
{

}

void MainWindow::on_treeView_intermittentStopButton_clicked()
{

}

void MainWindow::on_treeView_instantRowsToAddEdit_editingFinished()
{

}

void MainWindow::on_treeView_instantAddButton_clicked()
{

}

void MainWindow::on_treeView_instantClearButton_clicked()
{
}

void MainWindow::on_treeView_columnCountEdit_editingFinished()
{
    // TODO - set the tree view column count

    //treeViewModel->SetColumnCount( ui->treeView_columnCountEdit->value() );
    //ui->treeView->setVisible( false );
    //ui->treeView->setVisible( true );
}

void MainWindow::on_treeView_resizeColumnsButton_clicked()
{
    for( int i = 0; i < ui->treeView->model()->columnCount(); i++ )
        ui->treeView->resizeColumnToContents( i );
}

void MainWindow::on_treeView_expandAllButton_clicked()
{
    ui->treeView->expandAll();
}

void MainWindow::on_treeView_collapseAllButton_clicked()
{
    ui->treeView->collapseAll();
}

void MainWindow::on_treeView_sctionCountEdit_editingFinished()
{
    int newSectionCount = ui->treeView_sctionCountEdit->value();
    treeViewModel->SetSectionCount( newSectionCount );

    //treeViewModel->SetSectionCount( ui->treeView_sctionCountEdit->value() );

    // TODO - set maximum value of instant and timer section to add to values
}

void MainWindow::Log(QString message)
{
    if( ui->logStateEdit->isChecked() )
        ui->log->appendPlainText( message );
}

void MainWindow::SetStatus(QString status)
{
    statusBar()->showMessage( status );
}

void MainWindow::ClearStatus()
{
    statusBar()->clearMessage();
}

void MainWindow::UpdateRandomStringCountLabel()
{
    ui->randomStringCountLabel->setText( QString::number( randomStringManager->RandomStringCount() ) );
}

void MainWindow::UpdateTreeWidgetRowCountLabel()
{
    ui->treeWidget_rowCountLabel->setText( QString::number( treeWidgetRowCount ) );
}

void MainWindow::on_randomStringGenerateButton_clicked()
{
    QElapsedTimer timer;
    timer.start();

    int generateCount = ui->randomStringGenerationCount->value();
    SetStatus( QString ( "Generating %1 random strings..." ).arg( generateCount ) );

    for( int i = 0; i < generateCount; i++ )
    {
        int minLength = ui->randomStringMinLength->value();
        int maxLength = ui->randomStringMaxLength->value();

        int length = randomStringManager->RandomNumber( minLength, maxLength );

        randomStringManager->AppendRandomString( randomStringManager->RandomString( length ) );
    }

    qint64 msElapsed = timer.elapsed();
    Log( QString( "Finished generating random strings in %1 ms" ).arg( msElapsed ) );

    UpdateRandomStringCountLabel();
    ClearStatus();
}

void MainWindow::on_randomStringClearButton_clicked()
{
    randomStringManager->Clear();
    UpdateRandomStringCountLabel();
}

void MainWindow::on_clearLogButton_clicked()
{
    ui->log->clear();
}

void MainWindow::on_randomStringMinLength_editingFinished()
{
    randomStringManager->SetRandomStringGenerationRange( ui->randomStringMinLength->value(), ui->randomStringMaxLength->value() );
}

void MainWindow::on_randomStringMaxLength_editingFinished()
{
    randomStringManager->SetRandomStringGenerationRange( ui->randomStringMinLength->value(), ui->randomStringMaxLength->value() );
}

void MainWindow::on_randomStringPrependWithStringEdit_toggled(bool checked)
{
    bool prepend = ui->randomStringPrependWithStringEdit->isChecked();
    bool inject = ui->randomStringInjectWithStringEdit->isChecked();
    bool append = ui->randomStringAppendWithStringEdit->isChecked();

    randomStringManager->SetRandomStringModifierStates( prepend, inject, append );
}

void MainWindow::on_randomStringInjectWithStringEdit_toggled(bool checked)
{
    bool prepend = ui->randomStringPrependWithStringEdit->isChecked();
    bool inject = ui->randomStringInjectWithStringEdit->isChecked();
    bool append = ui->randomStringAppendWithStringEdit->isChecked();

    randomStringManager->SetRandomStringModifierStates( prepend, inject, append );
}

void MainWindow::on_randomStringAppendWithStringEdit_toggled(bool checked)
{
    bool prepend = ui->randomStringPrependWithStringEdit->isChecked();
    bool inject = ui->randomStringInjectWithStringEdit->isChecked();
    bool append = ui->randomStringAppendWithStringEdit->isChecked();

    randomStringManager->SetRandomStringModifierStates( checked, checked, checked );
}

void MainWindow::on_randomStringPrependWithStringValueEdit_textChanged(const QString &arg1)
{
    QString prepend = ui->randomStringPrependWithStringValueEdit->text();
    QString inject = ui->randomStringInjectWithStringValueEdit->text();
    QString append = ui->randomStringAppendWithStringValueEdit->text();

    randomStringManager->SetRandomStringModifiers( prepend, inject, append );
}

void MainWindow::on_randomStringInjectWithStringValueEdit_textChanged(const QString &arg1)
{
    QString prepend = ui->randomStringPrependWithStringValueEdit->text();
    QString inject = ui->randomStringInjectWithStringValueEdit->text();
    QString append = ui->randomStringAppendWithStringValueEdit->text();

    randomStringManager->SetRandomStringModifiers( prepend, inject, append );
}

void MainWindow::on_randomStringAppendWithStringValueEdit_textChanged(const QString &arg1)
{
    QString prepend = ui->randomStringPrependWithStringValueEdit->text();
    QString inject = ui->randomStringInjectWithStringValueEdit->text();
    QString append = ui->randomStringAppendWithStringValueEdit->text();

    randomStringManager->SetRandomStringModifiers( prepend, inject, append );
}
