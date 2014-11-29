#pragma once

#include <vector>

class QString;
class Move;

static const int EMPTY = 0;
static const int WHITE = 1;
static const int BLACK = 2;

struct TableData {
        bool whiteToMove;
        int table[24];
        int whiteHand;
        int blackHand;
};

struct Table {

        int mill[24][4];

        Table();
        Table(const Table* table);
        ~Table();
        TableData data;
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
        int getField(int idx);
        void setPos(int* table, int whiteHand, int blackHand, bool whiteToMove);
        void backupPosition(const Table* const table);
        void restorePosition(Table* const table) const;
        TableData getTableData();
};

