#ifndef SETUPSOURCEWINDOW_H
#define SETUPSOURCEWINDOW_H

#include <QDialog>
#include <QFileDialog>
#include <QVector>
#include <TaskProperties.h>
#include <QDebug>

#include <unordered_map>

#include "mainwindow.h"

namespace Ui {
class SetupSourceWindow;
}

class SetupSourceWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SetupSourceWindow(QWidget *parent = nullptr);
    ~SetupSourceWindow();

public slots:
    void checkSourceRequest();
    void randomSourceRequest();

    void loadFileRequest();

signals:
    void newSourceLoaded(TaskProperties* tp);


private:
    Ui::SetupSourceWindow *ui;

    int field_width;
    std::unordered_map<char, int> fig_to_count;

    TaskProperties* tp;
};

#endif // SETUPSOURCEWINDOW_H
