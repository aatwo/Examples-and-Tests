#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qsrand( QDateTime::currentMSecsSinceEpoch() );
    on_treeWidget_sectionCountEdit_editingFinished();
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
        headerLabels.append( RandomString( 5, 10 ) );
        ui->treeWidget->resizeColumnToContents( i );
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

    for( int i = 0; i < ui->treeWidget->columnCount(); i++ )
    {
        ui->treeWidget->resizeColumnToContents( i );
    }


    ui->treeWidget_instantSectionToAddToEdit->setMaximum( newSectionCount );
}

void MainWindow::on_treeWidget_intermittentRowsToAddEdit_editingFinished()
{

}

void MainWindow::on_treeWidget_intermittentFrequencyToAddEdit_editingFinished()
{

}

void MainWindow::on_treeWidget_intermittentStartButton_clicked()
{

}

void MainWindow::on_treeWidget_intermittentStopButton_clicked()
{

}

void MainWindow::on_treeWidget_instantRowsToAddEdit_editingFinished()
{

}

void MainWindow::on_treeWidget_instantAddButton_clicked()
{
    int rowCount = ui->treeWidget_instantRowsToAddEdit->value();
    QTreeWidgetItem* sectionItem = ui->treeWidget->topLevelItem( ui->treeWidget_instantSectionToAddToEdit->value() - 1 );

    for( int i = 0; i < rowCount; i++ )
    {
        QStringList rowData = RandomStringList( ui->treeWidget->columnCount(), 10, 20 );
        QTreeWidgetItem* item = new QTreeWidgetItem( rowData );

        sectionItem->addChild( item );
    }

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

}

void MainWindow::on_addItemTimerExpired()
{
    // TODO
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

    return ret;
}

QString MainWindow::RandomString(int minLength, int maxLength)
{
    int length = RandomNumber( minLength, maxLength );
    return RandomString( length );
}

QStringList MainWindow::RandomStringList(int stringCount, int minStringLength, int maxStringLength)
{
    QStringList ret;

    for( int i = 0; i < stringCount; i++ )
    {
        ret.append( RandomString( minStringLength, maxStringLength ) );
    }

    return ret;
}

QStringList MainWindow::RandomStringList(int stringCount, int stringLength)
{
    QStringList ret;

    for( int i = 0; i < stringCount; i++ )
    {
        ret.append( RandomString( stringLength ) );
    }

    return ret;
}

QStringList MainWindow::RandomStringList(int stringCount)
{
    QStringList ret;

    for( int i = 0; i < stringCount; i++ )
    {
        ret.append( RandomString( 15 ) );
    }

    return ret;
}
