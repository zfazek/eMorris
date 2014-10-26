#include "mill.h"
#include "node.h"
#include "move.h"
#include "table.h"
#include <stdio.h>
#include <iostream>
#include <QString>
#include <vector>
#include <time.h>

using namespace std;


Mill::Mill() {
    table = new Table();
    this->initTable(true);
    n = 500;
}

Mill::~Mill() {
    delete table;
}

/******************************************************************************
 *
 * These values should be reset before every new game
 *
 ******************************************************************************/
void Mill::initTable(bool historyClear) {
    table->initTable();
    if (historyClear) {
        history.clear();
        historyIdx = -1;
    }
}

vector<QString> Mill::getHistory() {
    return history;
}

int Mill::move(Move move, bool updateHistory) {
    int res = table->moveCheck(move, true);
    if (res == 0) {
        if (updateHistory) {
            int size = history.size() - historyIdx - 1;
            for (int i = 0; i < size; i++){
                history.pop_back();
            }
            history.push_back(QString::fromStdString(move.toString()));
            historyIdx = history.size() - 1;
        }
        return 0;
    } else {
        cout << "Invalid move: " << move.toString() << endl;
        return -1;
    }
}

int Mill::getHistoryIdx() {
    return historyIdx;
}

void Mill::setHistoryIdx(int idx) {
    historyIdx = idx;
}

/******************************************************************************
 *
 * Updates the table according to the history
 *
 ******************************************************************************/
void Mill::updateTable() {
    initTable(false);
    for (int i = 0; i <= historyIdx; i++) {
        table->moveCheck(Move::getMove(history[i]), true);
    }
}

string Mill::getBestMoveMCTS() {
    Node *move = new Node(this);
    Table backupTable;
    string bestMove = "";
    time_t start, end;
    time(&start);
    for (int i = 0; i < n; i++) {
        backupTable.backupPosition(table);
        move->selectAction();
        backupTable.restorePosition(table);
    }
    time(&end);
    bestMove = move->getBest()->currMove.toString();
    for (Node *c : move->getChildren()) {
        c->print();
    }
    printf("best move: ");
    move->getBest()->print();
    printf("elapsed time: %ld\n", end - start);
    delete move;
    return bestMove;
}
