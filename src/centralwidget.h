#pragma once

#include <QWidget>
#include <stdio.h>
#include <string>

class QGridLayout;
class QHBoxLayout;
class QLineEdit;
class QTextEdit;
class QLabel;
class QPushButton;
class QString;
class Table;
class Mill;
class Canvas;
class QListWidget;
class QListWidgetItem;

class CentralWidget : public QWidget {
    Q_OBJECT

    public:
        explicit CentralWidget();
        void printTable();

signals:

    private:
        QGridLayout* grid;
        QTextEdit* textEditHistory;
        QHBoxLayout* hboxStart;
        QHBoxLayout* hboxSend;
        QHBoxLayout* hboxWhiteHand;
        QHBoxLayout* hboxBlackHand;
        QHBoxLayout* hboxHistoryButton;
        QLineEdit* lineEditTurn;
        QLabel* labelHistory;
        QLabel* labelWhiteHand;
        QLabel* labelBlackHand;
        QLabel* labelMoves;
        QPushButton* buttonMove;
        QPushButton* buttonHistoryToStart;
        QPushButton* buttonHistoryToEnd;
        QPushButton* buttonHistoryPrev;
        QPushButton* buttonHistoryNext;
        QLabel* labels[24];
        QLabel* labelX[7];
        QLabel* labelY[7];
        Mill* mill;
        Canvas* canvas;
        QLabel* labelTable;
        QListWidget* listWidget;

        void initGui();
        void printHistory();
        void printGameOver(int isEnd);

        private slots:
            void historyToStart();
        void historyToEnd();
        void historyPrev();
        void historyNext();
        void MoveDoubleClicked(QListWidgetItem*);
        void makeMove();

};

