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
    vector<Move*> visited;
    Move *cur = this;
    visited.push_back(this);
    while (!cur->isLeaf()) {
        cur = cur->select();
        visited.push_back(cur);
    }
    cur->expand();
    Move *newNode = cur->select();
    visited.push_back(newNode);
    int value = evaluate();
    if (! mill->table->whiteToMove) {
        value = -value;
    }
    for (Move *node : visited) {
        node->updateStats(value);
    }
}

int Move::arity() {
    return children.size();
}

void Move::print() {
    printf("%-15.15s %3.0f / %3.0f\n",
            mill->convertMoveToCoord(currMove).c_str(),
            totValue,
            nVisits);
}

Move *Move::select() {
    Move *selected = nullptr;
    double bestValue = -10e10;
    for (Move *c : children) {
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

bool Move::isLeaf() {
    return children.size() == 0;
}

int Move::evaluate() {
    mill->backupPosition();
    for (int i = 0; i < 100; i++) {
        vector<string> moves = mill->getAllMoves();
        mill->move(QString::fromStdString(mill->convertMoveToCoord(moves[rand() % moves.size()])), false);

        // -1: Black won, 0: no end, 1: White won
        int end = mill->isEnd();
        if (end != 0) {
            mill->restorePosition();
            return end;
        }
    }
    mill->restorePosition();
    return 0; // no progress -> DRAW

    //return rand() % 3 - 1;
}

void Move::expand() {
    vector<string> moves = mill->getAllMoves();
    for (int i = 0; i < moves.size(); i++) {
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
