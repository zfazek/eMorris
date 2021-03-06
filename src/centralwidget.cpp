#include <QtGui>
#include <string>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include "centralwidget.h"
#include "mainwindow.h"
#include "table.h"
#include "mill.h"
#include "move.h"
#include "canvas.h"
#include <thread>
#include <unistd.h>

using namespace std;

CentralWidget::CentralWidget() {
    mill = new Mill();
    initGui();
}

void CentralWidget::setMainWindow(MainWindow* main) {
    mainWindow = main;
}

void CentralWidget::initGui() {
    grid = new QGridLayout();
    this->setLayout(grid);
    lineEditTurn = new QLineEdit("");
    lineEditTurn->setReadOnly(true);
    labelWhiteHand = new QLabel("");
    labelBlackHand = new QLabel("");
    hboxWhiteHand = new QHBoxLayout();
    hboxBlackHand = new QHBoxLayout();
    hboxMakeMove = new QHBoxLayout();
    cbTwoPlayer = new QCheckBox("2 Players");
    hboxWhiteHand->addWidget(labelWhiteHand);
    hboxBlackHand->addWidget(labelBlackHand);
    labelHistory = new QLabel("History: ");
    grid->addWidget(labelHistory, 0, 8, 1, 1);
    textEditHistory = new QTextEdit();
    textEditHistory->setReadOnly(true);
    grid->addWidget(textEditHistory, 1, 8, 16, 1);
    grid->addLayout(hboxWhiteHand, 17, 0, 1, 4);
    grid->addLayout(hboxBlackHand, 17, 4, 1, 4);
    grid->addWidget(lineEditTurn, 18, 0, 1, 8);
    buttonHistoryToStart = new QPushButton("|<");
    buttonHistoryToEnd = new QPushButton(">|");
    buttonHistoryPrev = new QPushButton("<");
    buttonHistoryNext = new QPushButton(">");
    hboxHistoryButton = new QHBoxLayout();
    this->connect(buttonHistoryToStart, SIGNAL(clicked()),
            this, SLOT(historyToStart()));
    this->connect(buttonHistoryToEnd, SIGNAL(clicked()),
            this, SLOT(historyToEnd()));
    this->connect(buttonHistoryPrev, SIGNAL(clicked()),
            this, SLOT(historyPrev()));
    this->connect(buttonHistoryNext, SIGNAL(clicked()),
            this, SLOT(historyNext()));
    hboxHistoryButton->addWidget(buttonHistoryToStart);
    hboxHistoryButton->addWidget(buttonHistoryPrev);
    hboxHistoryButton->addWidget(buttonHistoryNext);
    hboxHistoryButton->addWidget(buttonHistoryToEnd);
    grid->addLayout(hboxHistoryButton, 17, 8);
    canvas = new Canvas();
    canvas->setTable(mill);
    labelTable = new QLabel();
    grid->addWidget(labelTable, 0, 0, 16, 8);
    grid->addWidget(canvas, 0, 0, 16, 8);
    labelMoves = new QLabel("Possible moves:");
    grid->addWidget(labelMoves, 0, 9, 1, 1);
    listWidget = new QListWidget(this);
    this->connect(listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(MoveDoubleClicked(QListWidgetItem*)));
    //this->connect(listWidget, SIGNAL(itemActivated(QListWidgetItem*)),
    //        this, SLOT(MoveDoubleClicked(QListWidgetItem*)));
    grid->addWidget(listWidget, 1, 9, -1, 1);
    buttonMove = new QPushButton("make move");
    this->connect(buttonMove, SIGNAL(clicked()),
            this, SLOT(makeMove()));
    hboxMakeMove->addWidget(cbTwoPlayer);
    hboxMakeMove->addWidget(buttonMove);
    grid->addLayout(hboxMakeMove, 18, 8);
    printTable();
}

void CentralWidget::MoveDoubleClicked(QListWidgetItem *item) {
    mill->move(Move::getMove(item->text()), true);
    printTable();
    int isEnd = mill->table->isEnd();
    printGameOver(isEnd);
    if (isEnd != 0) {
        return;
    }
    if (! cbTwoPlayer->isChecked()) {
        makeMove();
    }
}

void CentralWidget::enableWidgets(bool enable) {
    listWidget->setEnabled(enable);
    buttonHistoryToStart->setEnabled(enable);
    buttonHistoryToEnd->setEnabled(enable);
    buttonHistoryPrev->setEnabled(enable);
    buttonHistoryNext->setEnabled(enable);
    buttonMove->setEnabled(enable);
    cbTwoPlayer->setEnabled(enable);
    mainWindow->enableMenus(enable);
}

void CentralWidget::makeMove() {
    enableWidgets(false);
    mill->thinking = true;
    std::thread t(&Mill::setBestMoveMCTS, mill);
    while (mill->thinking) {
        QCoreApplication::processEvents();
        usleep(300);
    }
    t.join();
    enableWidgets(true);
    string bestMove = mill->bestMoveStr;
    mill->move(Move::getMove(QString::fromStdString(bestMove)), true);
    printTable();
    int isEnd = mill->table->isEnd();
    printGameOver(isEnd);
}

void CentralWidget::printGameOver(int isEnd) {
    if (isEnd == 0) {
        return;
    }
    QMessageBox msgBox(this);
    if (isEnd == 1) 
        msgBox.setText("White Won!");
    else 
        msgBox.setText("Black Won!");
    msgBox.exec();
}

/******************************************************************************
 *
 * This slot is invoked when historyToStart button is pressed
 *
 ******************************************************************************/
void CentralWidget::historyToStart() {
    mill->setHistoryIdx(-1);
    mill->updateTable();
    printTable();
}

/******************************************************************************
 *
 * This slot is invoked when historyToEnd button is pressed
 *
 ******************************************************************************/
void CentralWidget::historyToEnd() {
    mill->setHistoryIdx(mill->getHistory().size() - 1);
    mill->updateTable();
    printTable();
}

/******************************************************************************
 *
 * This slot is invoked when historyPrev button is pressed
 *
 ******************************************************************************/
void CentralWidget::historyPrev() {
    if (mill->getHistoryIdx() > -1) {
        mill->setHistoryIdx(mill->getHistoryIdx() - 1);
        mill->updateTable();
        printTable();
    }
}

/******************************************************************************
 *
 * This slot is invoked when historyNext button is pressed
 *
 ******************************************************************************/
void CentralWidget::historyNext() {
    if (mill->getHistoryIdx() < (int)mill->getHistory().size() - 1 ||
            (mill->getHistoryIdx() == -1 && mill->getHistory().size() > 0)) {
        mill->setHistoryIdx(mill->getHistoryIdx() + 1);
        mill->updateTable();
        printTable();
    }
}

/******************************************************************************
 *
 * This method prints the history of the previous moves
 *
 ******************************************************************************/
void CentralWidget::printHistory() {
    vector<QString> history = mill->getHistory();
    int idx = mill->getHistoryIdx();
    int n = 1;
    QString s = "    White\t\tBlack\n";
    s += "    -----\t\t-----\n";
    int i = 0;
    while (i <= idx) {
        if (n < 10) 
            s += QString("%1. %2").arg(QString::number(n++), history[i]);
        else 
            s += QString("%1.%2").arg(QString::number(n++), history[i]);
        if (i < idx) {
            if (history[i].size() < 8)
                s += QString("\t\t%1\n").arg(history[++i]);
            else
                s += QString("\t%1\n").arg(history[++i]);
        } else {
            s += QString("\n");
        }
        i++;
    }
    textEditHistory->clear();
    QPalette *palette = new QPalette();
    palette->setColor(QPalette::Text,Qt::black);
    textEditHistory->setPalette(*palette);
    textEditHistory->setText(s);
    /*
    printf("idx: %d, i: %d, size: %d\n", idx, i, history.size());
    if (i < (int)history.size()) {
        QPalette *palette = new QPalette();
        palette->setColor(QPalette::Text,Qt::gray);
        textEditHistory->setPalette(*palette);
    }
    */
    QTextCursor c =  textEditHistory->textCursor();
    c.movePosition(QTextCursor::End);
    textEditHistory->setTextCursor(c);
}

/******************************************************************************
 *
 * This method prints the table
 *
 ******************************************************************************/
void CentralWidget::printTable() {
    if (mill->table->data.whiteToMove) {
        lineEditTurn->setText("White to move");
    } else {
        lineEditTurn->setText("Black to move");
    }
    labelWhiteHand->setText(QString("White hand: %1").arg(mill->table->getWhiteHand()));
    labelBlackHand->setText(QString("Black hand: %1").arg(mill->table->getBlackHand()));
    printHistory();
    listWidget->clear();
    vector<Move> moves = mill->table->getAllMoves();
    vector<string> moves1;
    for(vector<Move>::iterator it = moves.begin(); it != moves.end(); it++) {
        moves1.push_back((*it).toString());
    }
    sort(moves1.begin(), moves1.end());
    for(vector<string>::iterator it = moves1.begin(); it != moves1.end(); it++) {
        new QListWidgetItem(QString::fromStdString(*it), listWidget);
    }

    // Updates the canvas where the graphical table is
    canvas->repaint();
}

TableData CentralWidget::getTableData() {
    return mill->table->getTableData();
}

void CentralWidget::setTableData(TableData d) {
    mill->table->data.whiteHand = d.whiteHand;
    mill->table->data.blackHand = d.blackHand;
    mill->table->data.whiteToMove = d.whiteToMove;
    mill->table->initData.whiteHand = d.whiteHand;
    mill->table->initData.blackHand = d.blackHand;
    mill->table->initData.whiteToMove = d.whiteToMove;
    for (int i = 0; i < 24; i++) {
        mill->table->data.table[i] = d.table[i];
        mill->table->initData.table[i] = d.table[i];
    }
    mill->clearHistory();
    printTable();
}

