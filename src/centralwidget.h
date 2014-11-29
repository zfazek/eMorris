#pragma once

#include <QWidget>
#include <QThread>
#include <stdio.h>
#include <string>

class QGridLayout;
class QHBoxLayout;
class QCheckBox;
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
class TableData;
class MainWindow;

class CentralWidget : public QWidget {
    Q_OBJECT

    public:
        Mill* mill;

        CentralWidget();
        void printTable();
        TableData getTableData();
        void setTableData(TableData data);
        void setMainWindow(MainWindow* mainWindow);

signals:

    private:
        MainWindow* mainWindow;
        QGridLayout* grid;
        QTextEdit* textEditHistory;
        QHBoxLayout* hboxStart;
        QHBoxLayout* hboxSend;
        QHBoxLayout* hboxWhiteHand;
        QHBoxLayout* hboxBlackHand;
        QHBoxLayout* hboxMakeMove;
        QHBoxLayout* hboxHistoryButton;
        QCheckBox* cbTwoPlayer;
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
        Canvas* canvas;
        QLabel* labelTable;
        QListWidget* listWidget;

        void initGui();
        void printHistory();
        void printGameOver(int isEnd);
        void enableWidgets(bool enable);

        private slots:
            void historyToStart();
        void historyToEnd();
        void historyPrev();
        void historyNext();
        void MoveDoubleClicked(QListWidgetItem*);
        void makeMove();
};
