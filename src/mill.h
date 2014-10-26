#pragma once

#include <vector>
#include <string>

class QString;
class Table;
class Move;

class Mill {
    public:

        Mill();
        ~Mill();
        Table *table;
        void initTable(bool);
        int getCoordX(int);
        int getCoordY(int);
        int getField(int);
        int getWhiteHand();
        int getBlackHand();
        std::vector<QString> getHistory();
        int move(Move move, bool updateHistory);
        static Move getMove(QString);
        int getHistoryIdx();
        void setHistoryIdx(int idx);
        void updateTable();
        void setPos(int* table, int whiteHand, int blackHand, bool whiteToMove);
        std::string getBestMoveMCTS();
        void backupPosition();
        void backupPosition(const Mill *mill);
        void restorePosition();
        void restorePosition(Mill *mill);
        int n;

    private:
        std::vector<QString> history;
        int historyIdx;
        static int getIdx(int, int);
        void printMoves(std::vector<std::string>);

        bool whiteToMove_backup;
        int whiteHand_backup;
        int blackHand_backup;
        int t_backup[24];
};

