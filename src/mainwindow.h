#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "centralwidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private:
        CentralWidget* centralWidget;
};

#endif // MAINWINDOW_H
