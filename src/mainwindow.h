#pragma once

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

