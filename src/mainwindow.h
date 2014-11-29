#pragma once

#include <QMainWindow>
#include <QClipboard>
#include "centralwidget.h"

class QAction;
class QMenu;

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();
        void enableMenus(bool enable);

    private:
        CentralWidget* centralWidget;
        QMenu* fileMenu;
        QMenu* editMenu;
        QAction* copyPosAction;
        QAction* pastePosAction;
        QAction* newGameAction;
        QClipboard* clipboard;
        void clipboardParseError(QString clipboard, int idx);

        private slots:
            void newGame();
            void setPosFromClipboard();
            void copyPosToClipboard();

};

