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

int Mill::getCoordX(int i) {
    if (i >= 0 && i <= 23)
        return coordHelper[i * 3 + 2];
    else
        return -1;
}

int Mill::getCoordY(int i) {
    if (i >= 0 && i <= 23)
        return coordHelper[i * 3 + 1];
    else
        return -1;
}

int Mill::getField(int i) {
    return table->table[i];
}

vector<QString> Mill::getHistory() {
    return history;
}

Move Mill::getMove(QString input) {
    Move move;
    move.length = 0;
    int length = input.length();

    /* move d2 */
    if (length == 7) {
        int x = input.at(5).toAscii() - 'a';
        int y = 7 - input.mid(6, 1).toInt();
        int i = getIdx(x, y);
        if (i == -1) return move;
        move.length = 1;
        move.capture = false;
        move.x = i;
        return move;
    }

    /* move d2d3 */
    if (length == 9) {
        int x1 = input.at(5).toAscii() - 'a';
        int y1 = 7 - input.mid(6, 1).toInt();
        int i1 = getIdx(x1, y1);
        if (i1 == -1) return move;
        int x2 = input.at(7).toAscii() - 'a';
        int y2 = 7 - input.mid(8, 1).toInt();
        int i2 = getIdx(x2, y2);
        if (i2 == -1) return move;
        move.length = 2;
        move.capture = false;
        move.x = i1;
        move.y = i2;
        return move;
    }

    /* move d2,d3 */
    if (length == 10) {
        int x1 = input.at(5).toAscii() - 'a';
        int y1 = 7 - input.mid(6, 1).toInt();
        int i1 = getIdx(x1, y1);
        if (i1 == -1) return move;
        int x2 = input.at(8).toAscii() - 'a';
        int y2 = 7 - input.mid(9, 1).toInt();
        int i2 = getIdx(x2, y2);
        if (i2 == -1) return move;
        move.length = 2;
        move.capture = true;
        move.x = i1;
        move.y = i2;
        return move;
    }

    /* move a1d1,f4 */
    if (length == 12) {
        int x1 = input.at(5).toAscii() - 'a';
        int y1 = 7 - input.mid(6, 1).toInt();
        int i1 = getIdx(x1, y1);
        if (i1 == -1) return move;
        int x2 = input.at(7).toAscii() - 'a';
        int y2 = 7 - input.mid(8, 1).toInt();
        int i2 = getIdx(x2, y2);
        if (i2 == -1) return move;
        int comma = input.at(9).toAscii();
        if (comma != ',') return move;
        int x3 = input.at(10).toAscii() - 'a';
        int y3 = 7 - input.mid(11, 1).toInt();
        int i3 = getIdx(x3, y3);
        if (i3 == -1) return move;
        move.length = 3;
        move.capture = true;
        move.x = i1;
        move.y = i2;
        move.z = i3;
        return move;
    }
    return move;
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

/******************************************************************************
 *
 * Gets the index of the field from coordinates
 *
 ******************************************************************************/
int Mill::getIdx(int x, int y) {
    for (int i = 0; i < 24; ++i) {
        if (coordHelper[i * 3 + 1] == y &&
                coordHelper[i * 3 + 2] == x)
            return i;
    }
    return -1;
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
        table->moveCheck(getMove(history[i]), true);
    }
}

/******************************************************************************
 *
 * Sets any position. It is good for testing.
 * t contains the table
 *
 ******************************************************************************/
void Mill::setPos(int *t, int whiteHand, int blackHand, bool whiteToMove) {
    for (int i = 0; i < 24; i++)
        table->table[i] = *(t+i);
    table->whiteHand = whiteHand;
    table->blackHand = blackHand;
    table->whiteToMove = whiteToMove;
}

void Mill::backupPosition() {
    whiteToMove_backup = table->whiteToMove;
    whiteHand_backup = table->whiteHand;
    blackHand_backup = table->blackHand;
    for (int i = 0; i < 24; i++) t_backup[i] = table->table[i];
}

void Mill::backupPosition(const Mill *mill) {
    whiteToMove_backup = mill->table->whiteToMove;
    whiteHand_backup = mill->table->whiteHand;
    blackHand_backup = mill->table->blackHand;
    for (int i = 0; i < 24; i++) t_backup[i] = mill->table->table[i];
}

void Mill::restorePosition() {
    table->whiteToMove = whiteToMove_backup;
    table->whiteHand = whiteHand_backup;
    table->blackHand = blackHand_backup;
    for (int i = 0; i < 24; i++) table->table[i] = t_backup[i];
}

void Mill::restorePosition(Mill *mill) {
    mill->table->whiteToMove = whiteToMove_backup;
    mill->table->whiteHand = whiteHand_backup;
    mill->table->blackHand = blackHand_backup;
    for (int i = 0; i < 24; i++) mill->table->table[i] = t_backup[i];
}

string Mill::getBestMoveMCTS() {
    Node *move = new Node(this);
    string bestMove = "";
    time_t start, end;
    time(&start);
    for (int i = 0; i < n; i++) {
        backupPosition();
        move->selectAction();
        restorePosition();
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
