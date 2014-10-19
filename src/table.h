#pragma once

#include <vector>

class QString;

class Table {
    public:

        bool whiteToMove;
        Table();
        int getWhiteHand();
        int getBlackHand();
        int table[24];
        int whiteHand;
        int blackHand;
    private:
};

