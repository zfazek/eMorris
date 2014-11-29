#include <QtGui>
#include "mainwindow.h"
#include "centralwidget.h"
#include "table.h"
#include "mill.h"
#include <stdio.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    centralWidget = new CentralWidget();
    centralWidget->setMainWindow(this);
    this->setCentralWidget(centralWidget);
    this->setWindowTitle("eMorris");
    newGameAction = new QAction(tr("&New Game"), this);
    connect(newGameAction, SIGNAL(triggered()), this, SLOT(newGame()));
    copyPosAction = new QAction(tr("&Copy Position to Clipboard"), this);
    connect(copyPosAction, SIGNAL(triggered()), this, SLOT(copyPosToClipboard()));
    pastePosAction = new QAction(tr("&Paste Position from Clipboard"), this);
    connect(pastePosAction, SIGNAL(triggered()), this, SLOT(setPosFromClipboard()));
    fileMenu = menuBar()->addMenu(tr("&File"));
    editMenu = menuBar()->addMenu(tr("&Edit"));
    fileMenu->addAction(newGameAction);
    editMenu->addAction(copyPosAction);
    editMenu->addAction(pastePosAction);
    clipboard = QApplication::clipboard();
}

MainWindow::~MainWindow() {
}

void MainWindow::enableMenus(bool enable) {
    newGameAction->setEnabled(enable);
    copyPosAction->setEnabled(enable);
    pastePosAction->setEnabled(enable);
}

void MainWindow::copyPosToClipboard() {
    QString ret = "";
    TableData data = centralWidget->getTableData();
    for (int i = 0; i < 24; i++) {
        ret += QString::number(data.table[i]);
    }
    ret += " " + QString::number(data.whiteHand);
    ret += " " + QString::number(data.blackHand);
    ret += data.whiteToMove ? " w" : " b";
    ret += "\n";
    printf("%s\n", ret.toStdString().c_str());
    clipboard->setText(ret);
}

void MainWindow::setPosFromClipboard() {
    QString clip = clipboard->text();
    printf("%s\n", clip.toStdString().c_str());
    int t[24];
    for (int i = 0; i < 24; i++) {
        if (clip.at(i) < '0' || clip.at(i) > '2') {
            clipboardParseError(clip, i);
            return;
        }
        t[i] = clip.at(i).digitValue();
    }
    if (clip.at(24) != ' ') {
        clipboardParseError(clip, 24);
        return;
    }
    if (clip.at(25) < '0' || clip.at(25) > '9') {
        clipboardParseError(clip, 25);
        return;
    }
    int whiteHand = clip.at(25).digitValue();
    if (clip.at(26) != ' ') {
        clipboardParseError(clip, 26);
        return;
    }
    if (clip.at(27) < '0' || clip.at(27) > '9') {
        clipboardParseError(clip, 27);
        return;
    }
    int blackHand = clip.at(27).digitValue();
    if (clip.at(28) != ' ') {
        clipboardParseError(clip, 28);
        return;
    }
    if (! (clip.at(29) == 'w' || clip.at(29) == 'b')) {
        clipboardParseError(clip, 29);
        return;
    }
    bool whiteToMove = clip.at(29) == 'w' ? true : false;
    TableData data;
    for (int i = 0; i < 24; i++) {
        data.table[i] = t[i];
    }
    data.whiteHand = whiteHand;
    data.blackHand = blackHand;
    data.whiteToMove = whiteToMove;
    centralWidget->setTableData(data);
}

void MainWindow::clipboardParseError(QString clipboard, int idx) {
    printf("Parse error at %d: %s\n", idx, clipboard.toStdString().c_str());
}

void MainWindow::newGame() {
    centralWidget->mill->initTable(true);
    centralWidget->printTable();
}
