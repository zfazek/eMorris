#include "node.h"
#include "mill.h"
#include "move.h"
#include "table.h"
#include <vector>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <QString>

using namespace std;

Node::Node(Mill *m) {
    mill = m;
}

Node::~Node() {
    for (Node *c : children) {
        delete c;
    }
}

vector<Node*> Node::getChildren() {
    return children;
}

void Node::selectAction() {
    srand(time(0));
    vector<Node*> visited;
    Node *cur = this;
    visited.push_back(this);
    bool whiteToMove = mill->table->whiteToMove;
    while (!cur->isLeaf()) {
        cur = cur->select(visited.size());
        mill->move(cur->currMove, false);
        visited.push_back(cur);
    }
    Node *newNode;
    int value = mill->isEnd();
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
    for (Node *node : visited) {
        node->updateStats(value);
    }
}

Node *Node::select(int depth) {
    Node *selected = nullptr;
    double bestValue = -10e10;
    for (Node *c : children) {
        if (depth == 1 && c->nVisits < (double)mill->n / children.size() / 3 ) {
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

int Node::evaluate(const Node *newNode) {
    Mill m;
    m.backupPosition(mill);
    mill->move(newNode->currMove, false);
    int end = mill->isEnd();
    if (end != 0) {
        m.restorePosition(mill);
        return end;
    }
    for (int i = 0; i < MAX_LONG; i++) {
        vector<Move> moves = getTerminateMoves();
        mill->move(moves[rand() % moves.size()], false);

        // -1: Black won, 0: no end, 1: White won
        int end = mill->isEnd();
        if (end != 0) {
            m.restorePosition(mill);
            return end;
        }
    }
    m.restorePosition(mill);
    return 0; // no progress -> DRAW
}

int Node::arity() {
    return children.size();
}

void Node::print() {
    printf("%-15.15s %.0f/%.0f\n",
            currMove.toString().c_str(),
            totValue,
            nVisits);
}

bool Node::isLeaf() {
    return children.size() == 0;
}

vector<Move> Node::getTerminateMoves() {
    Mill m;
    vector<Move> moves = mill->getAllMoves();
    vector<Move> terminate_moves;
    for (Move move : moves) {
        m.backupPosition(mill);
        mill->move(move, false);
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

void Node::expand() {
    Mill m;
    vector<Move> moves = getTerminateMoves();
    for (size_t i = 0; i < moves.size(); i++) {
        Node *node= new Node(mill);
        node->currMove = moves[i];
        children.push_back(node);
    }
}

Node *Node::getBest() {
    Node *selected = nullptr;
    double bestValue = -10e10;
    for (Node *c : children) {
        if (c->nVisits > bestValue) {
            bestValue = c->nVisits;
            selected = c;
        }
    }
    return selected;
}

void Node::updateStats(int value) {
    nVisits++;
    totValue += value;
}

double Node::nextDouble() {
    return (double)rand() / RAND_MAX;
}
