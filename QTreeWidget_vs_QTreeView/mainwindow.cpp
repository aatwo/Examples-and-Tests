#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    randomStringManager( new CRandomStringManager )
{
    ui->setupUi(this);

    qsrand( QDateTime::currentMSecsSinceEpoch() );

    { // Setup TreeWidget

        ui->treeWidget->setUniformRowHeights( true );
        treeWidgetRowCount = 0;
        treeWidgetVisibleRowCount = 0;

        on_treeWidget_columnCountEdit_editingFinished();
        on_treeWidget_sectionCountEdit_editingFinished();
        on_treeWidget_intermittentFrequencyToAddEdit_editingFinished();

        treeWidgetFilterGroup = new CFilterGroup( ui->treeWidget_filterScrollArea );
        ui->treeWidget_filterScrollArea->setWidget( treeWidgetFilterGroup );
        ui->treeWidget_filterScrollArea->setWidgetResizable( true );

        connect( &treeWidgetTimer, &QTimer::timeout, this, &MainWindow::on_treeWidget_addItemTimerExpired );
        connect( treeWidgetFilterGroup, &CFilterGroup::FiltersChanged, this, &MainWindow::on_treeWidget_FiltersChanged );
    }

    { // Setup TreeView

        ui->treeView->setUniformRowHeights( true );
        treeViewRowCount = 0;

        QStringList headers;
        for( int i = 0; i < ui->treeView_columnCountEdit->value(); i++ )
            headers << randomStringManager->RandomString();

        treeViewFilterGroup = new CFilterGroup( ui->treeView_filterScrollArea );
        ui->treeView_filterScrollArea->setWidget( treeViewFilterGroup );
        ui->treeView_filterScrollArea->setWidgetResizable( true );

        treeViewModel = new CCustomModel( randomStringManager, headers, ui->treeView_sctionCountEdit->value(), ui->treeView );
        ui->treeView->setModel( treeViewModel );

        on_treeView_sctionCountEdit_editingFinished();

        connect( &treeViewTimer, &QTimer::timeout, this, &MainWindow::on_treeView_addItemTimerExpired );
        connect( treeViewFilterGroup, &CFilterGroup::FiltersChanged, this, &MainWindow::on_treeView_FiltersChanged );
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

    UpdateRandomStringCountLabel();
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

void MainWindow::on_treeWidget_intermittentFrequencyToAddEdit_editingFinished()
{
    treeWidgetTimer.setInterval( ui->treeWidget_intermittentFrequencyToAddEdit->value() );
}

void MainWindow::on_treeWidget_intermittentStartButton_clicked()
{
    ui->treeWidget_intermittentStopButton->setEnabled( true );
    ui->treeWidget_intermittentStartButton->setEnabled( false );
    treeWidgetTimer.start();
    treeWidgetElapsedTimer.restart();
}

void MainWindow::on_treeWidget_intermittentStopButton_clicked()
{
    ui->treeWidget_intermittentStopButton->setEnabled( false );
    ui->treeWidget_intermittentStartButton->setEnabled( true );
    treeWidgetTimer.stop();
}

void MainWindow::on_treeWidget_instantAddButton_clicked()
{
    QElapsedTimer timer;
    timer.start();

    int rowCount = ui->treeWidget_instantRowsToAddEdit->value();
    int columnCount = ui->treeWidget->columnCount();
    int requiredStringCount = rowCount * columnCount;

    if( requiredStringCount > randomStringManager->RandomStringCount() )
    {
        Log( QString( "Warning: string queue too small. %1 random strings will be generated..." ).arg( requiredStringCount - randomStringManager->RandomStringCount() ) );
    }

    SetStatus( QString ( "Adding %1 rows of data (%2 data items)..." ).arg( rowCount ).arg( requiredStringCount ) );

    int sectionValue = ui->treeWidget_instantSectionToAddToEdit->value();
    if( !sectionValue )
        sectionValue = randomStringManager->RandomNumber( 1, ui->treeWidget->topLevelItemCount() );

    sectionValue = sectionValue - 1;
    QTreeWidgetItem* sectionItem = ui->treeWidget->topLevelItem( sectionValue );

    CFilterGroup::FilterGroupDescription filters = treeWidgetFilterGroup->GetFilterGroupDescription();

    int newRowVisibleCount = 0;
    for( int i = 0; i < rowCount; i++ )
    {
        QStringList rowData = randomStringManager->RandomStringList( ui->treeWidget->columnCount() );
        QTreeWidgetItem* item = new QTreeWidgetItem( rowData );

        sectionItem->addChild( item );

        bool isVisible = IsTreeWidgetItemVisible( filters, item );
        item->setHidden( !isVisible );
        if( isVisible )
            newRowVisibleCount++;
    }

    qint64 msElapsed = timer.elapsed();
    Log( QString( "Finished adding data to the tree widget %1 ms" ).arg( msElapsed ) );

    treeWidgetVisibleRowCount += newRowVisibleCount;
    treeWidgetRowCount += rowCount;
    UpdateTreeWidgetRowCountLabel();
    UpdateRandomStringCountLabel();
    UpdateVisibleTreeWidgetRowCount();
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
    treeWidgetVisibleRowCount = 0;
    UpdateTreeWidgetRowCountLabel();
    UpdateVisibleTreeWidgetRowCount();
}

void MainWindow::on_treeWidget_addItemTimerExpired()
{
    qint64 timerSinceLastCall = treeWidgetElapsedTimer.elapsed();

    QElapsedTimer timer;
    timer.start();

    int rowCount = ui->treeWidget_intermittentRowsToAddEdit->value();
    int columnCount = ui->treeWidget->columnCount();
    int requireddStringCount = rowCount * columnCount;

    if( requireddStringCount > randomStringManager->RandomStringCount() )
    {
        Log( QString( "Warning: string queue too small. %1 random strings will be generated..." ).arg( requireddStringCount - randomStringManager->RandomStringCount() ) );
    }

    SetStatus( QString ( "Adding %1 rows of data (%2 data items)..." ).arg( rowCount ).arg( requireddStringCount ) );
    Log( QString( "Interval: %1 ms (%2 ms target)" ).arg( timerSinceLastCall ).arg( treeWidgetTimer.interval() ) );

    int sectionValue = ui->treeWidget_intermittentSectionToAddToValueEdit->value();
    if( !sectionValue )
        sectionValue = randomStringManager->RandomNumber( 1, ui->treeWidget->topLevelItemCount() );

    sectionValue = sectionValue - 1;
    QTreeWidgetItem* sectionItem = ui->treeWidget->topLevelItem( sectionValue );
    CFilterGroup::FilterGroupDescription filters = treeWidgetFilterGroup->GetFilterGroupDescription();

    int newVisibleRowCount = 0;
    for( int i = 0; i < rowCount; i++ )
    {
        QStringList rowData = randomStringManager->RandomStringList( ui->treeWidget->columnCount() );
        QTreeWidgetItem* item = new QTreeWidgetItem( rowData );

        sectionItem->addChild( item );

        bool isVisible = IsTreeWidgetItemVisible( filters, item );
        item->setHidden( !isVisible );

        if( isVisible )
            newVisibleRowCount++;
    }

    qint64 msElapsed = timer.elapsed();
    Log( QString( "Finished adding data to the tree widget %1 ms" ).arg( msElapsed ) );

    treeWidgetVisibleRowCount += newVisibleRowCount;
    treeWidgetRowCount += rowCount;
    UpdateTreeWidgetRowCountLabel();
    UpdateRandomStringCountLabel();
    UpdateVisibleTreeWidgetRowCount();
    ClearStatus();

    treeWidgetElapsedTimer.restart();
}

void MainWindow::on_treeWidget_expandAllButton_clicked()
{
    ui->treeWidget->expandAll();
}

void MainWindow::on_treeWidget_collapseAllButton_clicked()
{
    ui->treeWidget->collapseAll();
}

void MainWindow::on_treeWidget_addFilterButton_clicked()
{
    treeWidgetFilterGroup->AddFilterItem();
}

void MainWindow::on_treeWidget_clearFiltersButton_clicked()
{
    treeWidgetFilterGroup->Clear();
}

void MainWindow::on_treeWidget_FiltersChanged()
{
    QElapsedTimer timer;
    timer.start();

    CFilterGroup::FilterGroupDescription filterDescriptions = treeWidgetFilterGroup->GetFilterGroupDescription();
    int filterCount = filterDescriptions.filterItemDescriptions.size();

    SetStatus( QString( "Filtering the treeWidget (%1 filters to apply with %2 comparisons to make)..." ).arg( filterCount ).arg( filterCount * treeWidgetRowCount ) );

    treeWidgetVisibleRowCount = 0;
    for( int i = 0; i < ui->treeWidget->topLevelItemCount(); i++ )
    {
        QTreeWidgetItem* section = ui->treeWidget->topLevelItem( i );
        for( int j = 0; j < section->childCount(); j++ )
        {
            QTreeWidgetItem* row = section->child( j );
            bool rowVisible = IsTreeWidgetItemVisible( filterDescriptions, row );
            row->setHidden( !rowVisible );

            if( rowVisible )
                treeWidgetVisibleRowCount++;
        }
    }

    qint64 elapsedMs = timer.elapsed();
    Log( QString( "Finished filtering treeWidget in %1 ms" ).arg( elapsedMs ) );

    UpdateVisibleTreeWidgetRowCount();
}

void MainWindow::on_treeWidget_allowSortingEdit_toggled(bool checked)
{
    ui->treeWidget->setSortingEnabled( checked );
}

void MainWindow::on_treeView_intermittentFrequencyToAddEdit_editingFinished()
{
    treeViewTimer.setInterval( ui->treeView_intermittentFrequencyToAddEdit->value() );
    if( treeViewTimer.isActive() )
    {
        treeViewTimer.stop();
        treeViewTimer.start();
    }
}

void MainWindow::on_treeView_intermittentStartButton_clicked()
{
    ui->treeView_intermittentStopButton->setEnabled( true );
    ui->treeView_intermittentStartButton->setEnabled( false );
    treeViewTimer.start();
    treeViewElapsedTimer.restart();
}

void MainWindow::on_treeView_intermittentStopButton_clicked()
{
    ui->treeView_intermittentStopButton->setEnabled( false );
    ui->treeView_intermittentStartButton->setEnabled( true );
    treeViewTimer.stop();
}

void MainWindow::on_treeView_instantAddButton_clicked()
{
    QElapsedTimer timer;
    timer.start();

    int rowCount = ui->treeView_instantRowsToAddEdit->value();
    int sectionIndex = ui->treeView_instantSectionToAddToEdit->value();
    int requiredStringCount = rowCount * treeViewModel->columnCount();

    if( requiredStringCount > randomStringManager->RandomStringCount() )
    {
        Log( QString( "Warning: string queue too small. %1 random strings will be generated..." ).arg( requiredStringCount - randomStringManager->RandomStringCount() ) );
    }

    SetStatus( QString ( "Adding %1 rows of data (%2 data items)..." ).arg( rowCount ).arg( requiredStringCount ) );

    if( sectionIndex == 0 )
    {
        sectionIndex = randomStringManager->RandomNumber( 1, ui->treeView_sctionCountEdit->value() );
    }
    sectionIndex--;

    if( treeViewModel->insertRows( 0, rowCount, treeViewModel->index( sectionIndex, 0, QModelIndex() ) ) )
    {
        QModelIndex sectionModelIndex = treeViewModel->index( sectionIndex, 0, QModelIndex() );

        for( int i = 0; i < rowCount; i++ )
        {
            QModelIndex rowModelIndex = sectionModelIndex.child( i, 0 );
            CCustomModelItem* item = static_cast<CCustomModelItem*>( rowModelIndex.internalPointer() );

            for( int j = 0; j < item->columnCount(); j++ )
                item->setData( j, randomStringManager->RandomString() );
        }
    }

    qint64 msElapsed = timer.elapsed();
    Log( QString( "Finished adding data to the tree widget %1 ms" ).arg( msElapsed ) );

    treeViewRowCount += rowCount;
    UpdateTreeViewRowCountLabel();
    UpdateRandomStringCountLabel();
    ClearStatus();
}

void MainWindow::on_treeView_instantClearButton_clicked()
{
    treeViewModel->Clear();

    treeViewRowCount = 0;
    UpdateTreeViewRowCountLabel();
}

void MainWindow::on_treeView_columnCountEdit_editingFinished()
{
    int newColumnCount = ui->treeView_columnCountEdit->value();
    treeViewModel->SetColumnCount( newColumnCount );

    UpdateRandomStringCountLabel();
}

void MainWindow::on_treeView_resizeColumnsButton_clicked()
{
    for( int i = 0; i < ui->treeView->model()->columnCount(); i++ )
        ui->treeView->resizeColumnToContents( i );
}

void MainWindow::on_treeView_addItemTimerExpired()
{
    qint64 timeSinceLastCall = treeViewElapsedTimer.elapsed();

    QElapsedTimer timer;
    timer.start();

    int rowCount = ui->treeView_intermittentRowsToAddEdit->value();
    int sectionIndex = ui->treeView_intermittentSectionToAddToValueEdit->value();
    int requiredStringCount = rowCount * treeViewModel->columnCount();

    if( requiredStringCount > randomStringManager->RandomStringCount() )
    {
        Log( QString( "Warning: string queue too small. %1 random strings will be generated..." ).arg( requiredStringCount - randomStringManager->RandomStringCount() ) );
    }

    SetStatus( QString ( "Adding %1 rows of data (%2 data items)..." ).arg( rowCount ).arg( requiredStringCount ) );
    Log( QString( "Interval: %1 ms (%2 ms target)" ).arg( timeSinceLastCall ).arg( treeViewTimer.interval() ) );

    if( sectionIndex == 0 )
    {
        sectionIndex = randomStringManager->RandomNumber( 1, ui->treeView_sctionCountEdit->value() );
    }
    sectionIndex--;

    if( treeViewModel->insertRows( 0, rowCount, treeViewModel->index( sectionIndex, 0, QModelIndex() ) ) )
    {
        QModelIndex sectionModelIndex = treeViewModel->index( sectionIndex, 0, QModelIndex() );

        for( int i = 0; i < rowCount; i++ )
        {
            QModelIndex rowModelIndex = sectionModelIndex.child( i, 0 );
            CCustomModelItem* item = static_cast<CCustomModelItem*>( rowModelIndex.internalPointer() );

            for( int j = 0; j < item->columnCount(); j++ )
                item->setData( j, randomStringManager->RandomString() );
        }
    }

    qint64 msElapsed = timer.elapsed();
    Log( QString( "Finished adding data to the tree widget %1 ms" ).arg( msElapsed ) );

    treeViewRowCount += rowCount;
    UpdateTreeViewRowCountLabel();
    UpdateRandomStringCountLabel();
    ClearStatus();

    treeViewElapsedTimer.restart();
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

    ui->treeView_instantSectionToAddToEdit->setMaximum( newSectionCount );
    ui->treeView_intermittentSectionToAddToValueEdit->setMaximum( newSectionCount );
}

void MainWindow::on_treeView_clearFiltersButton_clicked()
{
    treeViewFilterGroup->Clear();
}

void MainWindow::on_treeView_addFilterButton_clicked()
{
    treeViewFilterGroup->AddFilterItem();
}

void MainWindow::on_treeView_FiltersChanged()
{
    // TODO
    qDebug() << "TODO - apply the filter";
}

void MainWindow::on_treeView_sortingEnabledEdit_toggled(bool checked)
{
    ui->treeView->setSortingEnabled( checked );
}

void MainWindow::Log(QString message)
{
    if( ui->logStateEdit->isChecked() )
        ui->log->appendPlainText( message );
}

void MainWindow::SetStatus(QString status)
{
    Log( status );
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

void MainWindow::UpdateTreeViewRowCountLabel()
{
    ui->treeView_rowCountLabel->setText( QString::number( treeViewRowCount ) );
}

bool MainWindow::IsTreeWidgetItemVisible(CFilterGroup::FilterGroupDescription& filterDescriptions_in, QTreeWidgetItem* item)
{
    bool rowVisible = true;

    for( int i = 0; i < filterDescriptions_in.filterItemDescriptions.size(); i++ )
    {
        int filterColumn = filterDescriptions_in.filterItemDescriptions.at( i ).first;
        QString filterText = filterDescriptions_in.filterItemDescriptions.at( i ).second;

        if( !item->text( filterColumn ).contains( filterText ) )
        {
            rowVisible = false;
            break;
        }
    }

    return rowVisible;
}

void MainWindow::UpdateVisibleTreeWidgetRowCount()
{
    ui->treeWidget_visibleRowCountLabel->setText( QString::number( treeWidgetVisibleRowCount ) );
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

void MainWindow::on_randomStringPrependWithStringEdit_toggled(bool )
{
    bool prepend = ui->randomStringPrependWithStringEdit->isChecked();
    bool inject = ui->randomStringInjectWithStringEdit->isChecked();
    bool append = ui->randomStringAppendWithStringEdit->isChecked();

    randomStringManager->SetRandomStringModifierStates( prepend, inject, append );
}

void MainWindow::on_randomStringInjectWithStringEdit_toggled(bool )
{
    bool prepend = ui->randomStringPrependWithStringEdit->isChecked();
    bool inject = ui->randomStringInjectWithStringEdit->isChecked();
    bool append = ui->randomStringAppendWithStringEdit->isChecked();

    randomStringManager->SetRandomStringModifierStates( prepend, inject, append );
}

void MainWindow::on_randomStringAppendWithStringEdit_toggled(bool )
{
    bool prepend = ui->randomStringPrependWithStringEdit->isChecked();
    bool inject = ui->randomStringInjectWithStringEdit->isChecked();
    bool append = ui->randomStringAppendWithStringEdit->isChecked();

    randomStringManager->SetRandomStringModifierStates( prepend, inject, append );
}

void MainWindow::on_randomStringPrependWithStringValueEdit_textChanged(const QString &)
{
    QString prepend = ui->randomStringPrependWithStringValueEdit->text();
    QString inject = ui->randomStringInjectWithStringValueEdit->text();
    QString append = ui->randomStringAppendWithStringValueEdit->text();

    randomStringManager->SetRandomStringModifiers( prepend, inject, append );
}

void MainWindow::on_randomStringInjectWithStringValueEdit_textChanged(const QString &)
{
    QString prepend = ui->randomStringPrependWithStringValueEdit->text();
    QString inject = ui->randomStringInjectWithStringValueEdit->text();
    QString append = ui->randomStringAppendWithStringValueEdit->text();

    randomStringManager->SetRandomStringModifiers( prepend, inject, append );
}

void MainWindow::on_randomStringAppendWithStringValueEdit_textChanged(const QString &)
{
    QString prepend = ui->randomStringPrependWithStringValueEdit->text();
    QString inject = ui->randomStringInjectWithStringValueEdit->text();
    QString append = ui->randomStringAppendWithStringValueEdit->text();

    randomStringManager->SetRandomStringModifiers( prepend, inject, append );
}
