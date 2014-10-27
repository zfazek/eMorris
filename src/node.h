#pragma once

#include <vector>
#include "move.h"

//class Mill;
class Table;
//class Move;

class Node {

    private:
        static const int nActions = 10;
        static constexpr double epsilon = 1e-6;
        static const int MAX_LONG = 100;
        std::vector<Node*> children;
        int idx = 0;
        Table *table;
        Table *backupTable;
        int n;

    public:

        bool operator==(const Node &other) {
            return currMove == other.currMove;
        }

        Move currMove;
        double nVisits;
        double totValue;

        Node(Table *table, int n);
        ~Node();
        std::vector<Node*> getChildren();
        void selectAction();
        int arity();
        Node *getBest();
        void print();

    private:

        void expand();
        Node *select(int depth);
        bool isLeaf();
        int evaluate(const Node *newNode);
        void updateStats(int value);
        double nextDouble();
        std::vector<Move> getTerminateMoves();

};

