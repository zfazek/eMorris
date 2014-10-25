#pragma once

#include <vector>
#include <string>

class QString;
class Table;
class Move;

static const int EMPTY = 0;
static const int WHITE = 1;
static const int BLACK = 2;

class Mill {
    public:

        Mill();
        ~Mill();
        Table *table;
        void initTable(bool);
        void initRules();
        int getCoordX(int);
        int getCoordY(int);
        int getField(int);
        int getWhiteHand();
        int getBlackHand();
        std::vector<QString> getHistory();
        int move(QString move, bool updateHistory);
        int move(Move move, bool updateHistory);
        int moveCheck(QString, bool updateHistory);
        int moveCheck(Move move, bool updateHistory);
        int getHistoryIdx();
        void setHistoryIdx(int);
        void updateTable();
        void printTable();
        void setPos(int* table, int whiteHand, int blackHand, bool whiteToMove);
        int moveCheck(int x, bool makeMove);
        int moveCheck(int x, int y, bool makeMove);
        int moveCheck(int x, int y, int z, bool makeMove);
        std::vector<std::string> getAllMoves();
        std::vector<Move> getAllMovesNew();
        std::string convertMoveToCoord(std::string);
        int isEnd();
        std::string getBestMoveMCTS();
        void backupPosition();
        void backupPosition(const Mill *mill);
        void restorePosition();
        void restorePosition(Mill *mill);
        int n;

    private:
        int mill[24][4];
        std::vector<QString> history;
        std::vector<Move> historyNew;
        int historyIdx;
        int getIdx(int, int);
        bool isMill(int idx, int color);
        bool hasSoloMorris(int);
        bool isNeighbor(int, int);
        int getNofPiece(int);
        void printMoves(std::vector<std::string>);

        bool whiteToMove_backup;
        int whiteHand_backup;
        int blackHand_backup;
        int t_backup[24];
};

