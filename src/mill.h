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
        int getWhiteHand();
        int getBlackHand();
        std::vector<QString> getHistory();
        int move(Move move, bool updateHistory);
        int getHistoryIdx();
        void setHistoryIdx(int idx);
        void updateTable();
        std::string getBestMoveMCTS();
        void backupPosition(const Mill *mill);
        void restorePosition(Mill *mill);
        int n;

    private:
        std::vector<QString> history;
        int historyIdx;
        void printMoves(std::vector<std::string>);

        bool whiteToMove_backup;
        int whiteHand_backup;
        int blackHand_backup;
        int t_backup[24];
};

