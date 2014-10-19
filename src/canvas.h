#pragma once

#include "table.h"
#include "mill.h"
#include <QWidget>

class Canvas : public QWidget {
    Q_OBJECT

    public:
        Canvas(QWidget* parent = 0);
        void setTable(Mill*);

    protected:
        Mill* mill;
        void paintEvent(QPaintEvent* event);


};

