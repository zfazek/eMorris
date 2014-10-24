#include "move.h"
#include "mill.h"
#include "table.h"
#include <vector>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <QString>

using namespace std;

int n = 0;

Move::Move(Mill *m) {
    mill = m;
    currMove = "";
}

Move::~Move() {
    for (Move *c : children) {
        delete c;
    }
}

vector<Move*> Move::getChildren() {
    return children;
}

void Move::selectAction() {
    srand(time(0));
    vector<Move*> visited;
    Move *cur = this;
    visited.push_back(this);
    bool whiteToMove = mill->table->whiteToMove;
    while (!cur->isLeaf()) {
        cur = cur->select(visited.size());
        mill->move(QString::fromStdString(mill->convertMoveToCoord(cur->currMove)), false);
        //printf("cur->select(visited.size()): ");cur->print();
        visited.push_back(cur);
    }
    Move *newNode;
    int value = MAX_LONG * mill->isEnd();
    if (value == 0) {
        cur->expand();
        newNode = cur->select(visited.size());
        visited.push_back(newNode);
        value = evaluate(newNode);
        if (! whiteToMove) {
            value = -value;
        }
    } else {
        if (! whiteToMove) {
            value = -value;
        }
        newNode = cur;
    }
    //printf("currMove: %s: %d, visited.size(): %d\n", mill->convertMoveToCoord(newNode->currMove).c_str(), value, visited.size());
    for (Move *node : visited) {
        node->updateStats(value);
    }
    //puts("");
}

Move *Move::select(int depth) {
    Move *selected = nullptr;
    double bestValue = -10e10;
    for (Move *c : children) {
        if (depth == 1 && c->nVisits < (double)mill->n / children.size() / 2 ) {
            return c;
        }
        double uctValue =
            c->totValue / (c->nVisits + epsilon) +
            sqrt(log(nVisits + 1) / (c->nVisits + epsilon)) +
            nextDouble() * epsilon;
        if (uctValue > bestValue) {
            selected = c;
            bestValue = uctValue;
        }
    }
    return selected;
}

int Move::evaluate(const Move *newNode) {
    Mill m;
    m.backupPosition(mill);
    mill->move(QString::fromStdString(mill->convertMoveToCoord(newNode->currMove)), false);
    int end = mill->isEnd();
    if (end != 0) {
        m.restorePosition(mill);
        //printf("orig end: %d\n", end);
        return MAX_LONG * end;
    }
    for (int i = 0; i < MAX_LONG; i++) {
        vector<string> moves = getTerminateMoves();
        mill->move(QString::fromStdString(mill->convertMoveToCoord(moves[rand() % moves.size()])), false);

        // -1: Black won, 0: no end, 1: White won
        int end = mill->isEnd();
        if (end != 0) {
            m.restorePosition(mill);
            return (MAX_LONG - i) * end;
        }
    }
    m.restorePosition(mill);
    return 0; // no progress -> DRAW

    //return rand() % 3 - 1;
}

int Move::arity() {
    return children.size();
}

void Move::print() {
    printf("%-15.15s %.0f/%.0f\n",
            mill->convertMoveToCoord(currMove).c_str(),
            totValue,
            nVisits);
}

bool Move::isLeaf() {
    return children.size() == 0;
}

vector<string> Move::getTerminateMoves() {
    Mill m;
    vector<string> moves = mill->getAllMoves();
    vector<string> terminate_moves;
    for (string move : moves) {
        m.backupPosition(mill);
        //mill->printTable();
        //printf("expand %s\n", mill->convertMoveToCoord(move).c_str());
        mill->move(QString::fromStdString(mill->convertMoveToCoord(move)), false);
        if (mill->isEnd() != 0) {
            terminate_moves.push_back(move);
        }
        m.restorePosition(mill);
    }
    if (terminate_moves.size() == 0) {
        return moves;
    } else {
        return terminate_moves;
    }
}

void Move::expand() {
    Mill m;
    vector<string> moves = getTerminateMoves();
    for (size_t i = 0; i < moves.size(); i++) {
        Move *move = new Move(mill);
        n++;
        move->idx = n;
        move->currMove = moves[i];
        children.push_back(move);
    }
}

Move *Move::getBest() {
    Move *selected = nullptr;
    double bestValue = -10e10;
    for (Move *c : children) {
        if (c->nVisits > bestValue) {
            bestValue = c->nVisits;
            selected = c;
        }
    }
    return selected;
}

void Move::updateStats(int value) {
    nVisits++;
    totValue += value;
    //print();
}

double Move::nextDouble() {
    return (double)rand() / RAND_MAX;
}

/*

   int main() {
   srand(time(0));
   Move *move = new Move(mill);
   for (int i = 0; i < 1e3; i++) {
   move->selectAction();
   }
   move->print();
   for (Move *c : move->getChildren()) {
   c->print();
   }
   move->getBest()->print();
   delete move;
   return 0;
   }
   */
