#pragma once

#include <vector>
#include <string>

class QString;
class Table;


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
        int move(QString, bool);
        int moveCheck(QString, bool);
        int getHistoryIdx();
        void setHistoryIdx(int);
        void updateTable();
        void printTable();
        void setPos(int*, int, int, bool);
        int moveCheck(int, bool);
        int moveCheck(int, int, bool);
        int moveCheck(int, int, int, bool);
        std::vector<std::string> getAllMoves();
        std::string convertMoveToCoord(std::string);
        int isEnd();
        int getNofNeighbors(int);
        std::string getBestMoveMCTS();
        void backupPosition();
        void backupPosition(const Mill *mill);
        void restorePosition();
        void restorePosition(Mill *mill);
        int n;

    private:
        int mill[24][4];
        std::vector<QString> history;
        int historyIdx;
        int getIdx(int, int);
        bool isMill(int, int);
        bool hasSoloMorris(int);
        bool isNeighbor(int, int);
        int getNofPiece(int);
        void printMoves(std::vector<std::string>);

        bool whiteToMove_backup;
        int whiteHand_backup;
        int blackHand_backup;
        int t_backup[24];
};

