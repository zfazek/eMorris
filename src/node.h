#pragma once

#include <vector>
#include "move.h"

class Mill;
//class Move;

class Node {

    private:
        static const int nActions = 10;
        static constexpr double epsilon = 1e-6;
        static const int MAX_LONG = 100;
        std::vector<Node*> children;
        double nVisits = 0;
        double totValue = 0;
        int idx = 0;
        Mill *mill;


    public:

        Node(Mill *mill);
        ~Node();
        std::vector<Node*> getChildren();
        void selectAction();
        int arity();
        Node *getBest();
        void print();

        Move currMove;

    private:

        void expand();
        Node *select(int depth);
        bool isLeaf();
        int evaluate(const Node *newNode);
        void updateStats(int value);
        double nextDouble();
        std::vector<Move> getTerminateMoves();

};

