#include "mainwindow.h"
#include "centralwidget.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    centralWidget = new CentralWidget();
    this->setCentralWidget(centralWidget);
    this->setWindowTitle("eMorris");
}

MainWindow::~MainWindow() {
}
