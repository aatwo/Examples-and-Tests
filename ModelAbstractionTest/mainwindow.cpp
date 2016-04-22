#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    CCustomModel* model = new CCustomModel( this );
    ui->treeView->setModel( model );
}

MainWindow::~MainWindow()
{
    delete ui;
}
