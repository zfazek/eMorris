#ifndef CANVAS_H
#define CANVAS_H

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

#endif // CANVAS_H
