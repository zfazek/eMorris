#pragma once

#include <vector>

class QString;

static const int EMPTY = 0;
static const int WHITE = 1;
static const int BLACK = 2;

struct Table {
        bool whiteToMove;
        Table();
        int getWhiteHand();
        int getBlackHand();
        int table[24];
        int whiteHand;
        int blackHand;
        void printTable();
        void initTable();
};

