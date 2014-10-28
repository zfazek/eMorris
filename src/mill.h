#pragma once

#include <vector>
#include <string>

class QString;
class Table;
class Move;
class Node;

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
        void setBestMoveMCTS();
        void getBestMoveOneThread(Node *move);
        void backupPosition(const Mill *mill);
        void restorePosition(Mill *mill);
        int n;
        std::string bestMoveStr;
        bool thinking;
        bool debug;

    private:
        std::vector<QString> history;
        int historyIdx;
        void printMoves(std::vector<std::string>);
};

