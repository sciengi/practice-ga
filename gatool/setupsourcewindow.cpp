#include "setupsourcewindow.h"
#include "ui_setupsourcewindow.h"

SetupSourceWindow::SetupSourceWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupSourceWindow)
{
    ui->setupUi(this);

    auto tp = new TaskProperties;

    connect(ui->loadButton, &QPushButton::clicked, this, &SetupSourceWindow::checkSourceRequest);
    connect(ui->closeButton, &QPushButton::clicked, this, &SetupSourceWindow::close);
    connect(ui->randomButton, &QPushButton::clicked, this, &SetupSourceWindow::randomSourceRequest);

    connect(ui->browseButton, &QPushButton::clicked, this, &SetupSourceWindow::loadFileRequest);
}

SetupSourceWindow::~SetupSourceWindow()
{
    delete ui;
}

void SetupSourceWindow::checkSourceRequest()
{
    qDebug() << "Checking task";



    emit newSourceLoaded(nullptr);
}

void SetupSourceWindow::randomSourceRequest()
{
    qDebug() << "Randoming task";
}


void SetupSourceWindow::loadFileRequest()
{
    qDebug() << "Loading a file";
    QString filename = QFileDialog::getOpenFileName(this, "Select a file");
    if (!filename.isEmpty())
    {
        // read file
        qDebug() << "Choosen " << filename;
    }
}
