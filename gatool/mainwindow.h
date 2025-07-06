#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QDebug>

#include <GA.h>

#include "setupsourcewindow.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void setupSourceRequest();
    void setupSourceLoaded(TaskProperties* tp);

signals:

private:
    Ui::MainWindow *ui;

    TaskProperties tp;
};
#endif // MAINWINDOW_H
