#pragma once

#include <vector>

class QString;

struct Table {
        bool whiteToMove;
        Table();
        int getWhiteHand();
        int getBlackHand();
        int table[24];
        int whiteHand;
        int blackHand;
};

