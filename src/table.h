#ifndef TABLE_H
#define TABLE_H

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

#endif // TABLE_H
