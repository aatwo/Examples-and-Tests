#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    box2DSimulationWidget = new Box2DSimulationWidget(this);
    ui->gridLayout->addWidget( box2DSimulationWidget );
}

MainWindow::~MainWindow()
{
    delete ui;
}
