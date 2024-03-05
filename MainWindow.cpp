#include "MainWindow.h"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->centralwidget->setMinimumSize(1800, 950);
    ui->widget->move(0, 0);
    ui->widget->setMinimumSize(1800, 950);
    ui->widget->setupWidget();
}

MainWindow::~MainWindow()
{
    delete ui;
}
