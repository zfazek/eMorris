#pragma once

#include <vector>

class QString;
class Move;

static const int EMPTY = 0;
static const int WHITE = 1;
static const int BLACK = 2;

struct Table {
        bool whiteToMove;
        int table[24];
        int whiteHand;
        int blackHand;
        int mill[24][4];

        Table();
        int getWhiteHand();
        int getBlackHand();
        void initRules();
        void printTable();
        void initTable();
        bool isMill(int idx, int color);
        int moveCheck(Move move, bool updateHistory);
        int moveCheck(int x, bool makeMove);
        int moveCheck(int x, int y, bool makeMove, int n_white, int n_black);
        int moveCheck(int x, int y, int z, bool makeMove, int n_white, int n_black);
        int getNofPiece(int);
        int isEnd();
        bool isNeighbor(int, int);
        bool hasSoloMorris(int);
        std::vector<Move> getAllMoves();

};

