#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    treeWidgetRowCount = 0;
    qsrand( QDateTime::currentMSecsSinceEpoch() );

    on_treeWidget_columnCountEdit_editingFinished();
    on_treeWidget_sectionCountEdit_editingFinished();
    on_treeWidget_intermittentFrequencyToAddEdit_editingFinished();

    connect( &treeWidgetTimer, &QTimer::timeout, this, &MainWindow::on_treeWidget_addItemTimerExpired );
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
        headerLabels.append( RandomString() );
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

    if( randomStringGenerateCount > randomStringQueue.size() )
    {
        Log( QString( "Warning: string queue too small. %1 random strings will be generated..." ).arg( randomStringGenerateCount - randomStringQueue.size() ) );
    }

    SetStatus( QString ( "Adding %1 rows of data..." ).arg( randomStringGenerateCount ) );

    int sectionValue = ui->treeWidget_instantSectionToAddToEdit->value();
    if( !sectionValue )
        sectionValue = RandomNumber( 1, ui->treeWidget->topLevelItemCount() );

    sectionValue = sectionValue - 1;
    QTreeWidgetItem* sectionItem = ui->treeWidget->topLevelItem( sectionValue );

    for( int i = 0; i < rowCount; i++ )
    {
        QStringList rowData = RandomStringList( ui->treeWidget->columnCount() );
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

    if( randomStringGenerateCount > randomStringQueue.size() )
    {
        Log( QString( "Warning: string queue too small. %1 random strings will be generated..." ).arg( randomStringGenerateCount - randomStringQueue.size() ) );
    }

    SetStatus( QString ( "Adding %1 rows of data..." ).arg( randomStringGenerateCount ) );

    int sectionValue = ui->treeWidget_intermittentSectionToAddToValueEdit->value();
    if( !sectionValue )
        sectionValue = RandomNumber( 1, ui->treeWidget->topLevelItemCount() );

    sectionValue = sectionValue - 1;
    QTreeWidgetItem* sectionItem = ui->treeWidget->topLevelItem( sectionValue );

    for( int i = 0; i < rowCount; i++ )
    {
        QStringList rowData = RandomStringList( ui->treeWidget->columnCount() );
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

int MainWindow::RandomNumber(int min, int max)
{
    return ( qrand() % ( max - min + 1 ) + min );
}

QString MainWindow::RandomString(int length)
{
    const int minAsciiValue = 97;
    const int maxAsciiValue = 122;

    QString ret;

    for( int i = 0; i < length; i++ )
    {
        char c = static_cast<char>( RandomNumber( minAsciiValue, maxAsciiValue ) );
        ret.append( c );
    }

    if( ui->randomStringPrependWithStringEdit->isChecked() )
    {
        ret.prepend( ui->randomStringPrependWithStringValueEdit->text() );
    }

    if( ui->randomStringInjectWithStringEdit->isChecked() )
    {
        ret.insert( ret.length() / 2, ui->randomStringInjectWithStringValueEdit->text() );
    }

    if( ui->randomStringAppendWithStringEdit->isChecked() )
    {
        ret.append( ui->randomStringAppendWithStringValueEdit->text() );
    }

    return ret;
}

QString MainWindow::RandomString()
{
    if( !randomStringQueue.isEmpty() )
        return randomStringQueue.dequeue();

    int minLength = ui->randomStringMinLength->value();
    int maxLength = ui->randomStringMaxLength->value();

    int length = RandomNumber( minLength, maxLength );

    return RandomString( length );
}

QStringList MainWindow::RandomStringList(int stringCount)
{
    QStringList ret;

    for( int i = 0; i < stringCount; i++ )
    {
        ret.append( RandomString() );
    }

    return ret;
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
    ui->randomStringCountLabel->setText( QString::number( randomStringQueue.size() ) );
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

        int length = RandomNumber( minLength, maxLength );

        randomStringQueue.append( RandomString( length ) );
    }

    qint64 msElapsed = timer.elapsed();
    Log( QString( "Finished generating random strings in %1 ms" ).arg( msElapsed ) );

    UpdateRandomStringCountLabel();
    ClearStatus();
}

void MainWindow::on_randomStringClearButton_clicked()
{
    randomStringQueue.clear();
    UpdateRandomStringCountLabel();
}

void MainWindow::on_clearLogButton_clicked()
{
    ui->log->clear();
}
