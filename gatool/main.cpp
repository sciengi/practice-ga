#include "mainwindow.h"

#include <QApplication>
#include <GA.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;


    w.show();
    return a.exec();
}
