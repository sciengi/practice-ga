#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->srcSetup, &QPushButton::clicked, this, &MainWindow::setupSourceRequest);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setupSourceRequest()
{
    auto* ssw = new SetupSourceWindow(this);
    connect(ssw, &SetupSourceWindow::newSourceLoaded, this, &MainWindow::setupSourceLoaded);
    ssw->exec();
}


void MainWindow::setupSourceLoaded(TaskProperties* tp)
{
    qDebug() << "Load solution!";
}
