#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "box2dsimulationwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;

    Box2DSimulationWidget* box2DSimulationWidget;
};

#endif // MAINWINDOW_H
