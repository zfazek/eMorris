#include "mill.h"
#include "node.h"
#include "move.h"
#include "table.h"
#include <stdio.h>
#include <iostream>
#include <QString>
#include <vector>
#include <time.h>
#include <unordered_map>
#include <thread>

using namespace std;


Mill::Mill() {
    table = new Table();
    this->initTable(true);
    n = 500;
    debug = false;
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

struct Point {
    double totValue;
    double nVisits;

    Point() {
        totValue = 0.0;
        nVisits = 0.0;
    }
};

struct MovetHasher {
    size_t operator()(const Move &m) const {
        return (size_t)(m.x + m.y + m.z + m.length);
    }
};

void Mill::getBestMoveOneThread(Node *move) {
    for (int i = 0; i < n; i++) {
        move->selectAction();
    }
}

void Mill::setBestMoveMCTS() {
    time_t start, end;
    int n_thread = thread::hardware_concurrency();
    vector<Node*> nodes;
    vector<thread> threads;
    vector<Table*> tables;
    time(&start);
    for (int i = 0; i < n_thread; i++) {
        Table *t = new Table(table);
        tables.push_back(t);
        Node *node = new Node(t, n);
        nodes.push_back(node);
        threads.push_back(thread(&Mill::getBestMoveOneThread, this, nodes[i]));
    }
    for (thread &t : threads) {
        t.join();
    }
    unordered_map<Move, Point, MovetHasher> children;
    for (int i = 0; i < n_thread; i++) {
        for (const Node *c : nodes[i]->getChildren()) {
            children[c->currMove].totValue += c->totValue;
            children[c->currMove].nVisits += c->nVisits;
        }
    }
    Move bestMove;
    double bestVisit = 0;
    double totValue = 0;
    double nVisits = 0;
    for (const auto &it : children) {
        if (it.second.nVisits > bestVisit) {
            bestVisit = it.second.nVisits;
            bestMove = it.first;
        }
        if (debug) {
        printf("%s %.0f/%.0f\n",
                it.first.toString().c_str(),
                it.second.totValue,
                it.second.nVisits);
        }
        totValue += it.second.totValue;
        nVisits += it.second.nVisits;
    }
    time(&end);
    if (debug) {
        printf("best move: %s (%.0f/%.0f)\n", bestMove.toString().c_str(), totValue, nVisits);
        printf("elapsed time: %ld\n", end - start);
    }
    for (Node *node : nodes) {
        delete node;
    }
    for (Table* table : tables) {
        delete table;
    }
    bestMoveStr =  bestMove.toString();
    thinking = false;
}

void Mill::clearHistory() {
    setHistoryIdx(-1);
    history.clear();
}
