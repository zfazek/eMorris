#include "canvas.h"
#include "table.h"
#include "mill.h"
#include <QtGui>
#include <QPainter>

Canvas::Canvas(QWidget* parent) : QWidget(parent) {
}

void Canvas::setTable(Mill* t) {
    mill = t;
}

/******************************************************************************
 *
 * This method prints the graphical table
 * You can change the size of the table by altering L variable
 *
 ******************************************************************************/
void Canvas::paintEvent(QPaintEvent* event) {
    int coord[] = {
        0,  2,  2,
        1,  8,  2,
        2, 14,  2,
        3,  4,  4,
        4,  8,  4,
        5, 12, 4,
        6, 6, 6,
        7, 8, 6,
        8, 10, 6,
        9, 2, 8,
        10, 4, 8,
        11, 6, 8,
        12, 10, 8,
        13, 12, 8,
        14, 14, 8,
        15, 6, 10,
        16, 8, 10,
        17, 10, 10,
        18, 4, 12,
        19, 8, 12,
        20, 12, 12,
        21, 2, 14,
        22, 8, 14,
        23, 14, 14
    };
    QPainter qp(this);
    qp.setRenderHint(QPainter::Antialiasing);
    QPen pen(Qt::black /*, Qt::SolidLine*/);
    qp.setPen(pen);
    int L = 12;
    qp.drawText(QPoint(2*L, 16*L), "A");
    qp.drawText(QPoint(4*L, 16*L), "B");
    qp.drawText(QPoint(6*L, 16*L), "C");
    qp.drawText(QPoint(8*L, 16*L), "D");
    qp.drawText(QPoint(10*L, 16*L), "E");
    qp.drawText(QPoint(12*L, 16*L), "F");
    qp.drawText(QPoint(14*L, 16*L), "G");
    qp.drawText(QPoint(1*L, 2*L), "7");
    qp.drawText(QPoint(1*L, 4*L), "6");
    qp.drawText(QPoint(1*L, 6*L), "5");
    qp.drawText(QPoint(1*L, 8*L), "4");
    qp.drawText(QPoint(1*L, 10*L), "3");
    qp.drawText(QPoint(1*L, 12*L), "2");
    qp.drawText(QPoint(1*L, 14*L), "1");
    qp.drawLine(2*L, 2*L, 14*L, 2*L);
    qp.drawLine(2*L, 14*L, 14*L, 14*L);
    qp.drawLine(2*L, 2*L, 2*L, 14*L);
    qp.drawLine(14*L, 2*L, 14*L, 14*L);
    qp.drawLine(4*L, 4*L, 12*L, 4*L);
    qp.drawLine(4*L, 4*L, 4*L, 12*L);
    qp.drawLine(4*L, 12*L, 12*L, 12*L);
    qp.drawLine(12*L, 4*L, 12*L, 12*L);
    qp.drawLine(6*L, 6*L, 10*L, 6*L);
    qp.drawLine(6*L, 6*L, 6*L, 10*L);
    qp.drawLine(6*L, 10*L, 10*L, 10*L);
    qp.drawLine(10*L, 6*L, 10*L, 10*L);
    qp.drawLine(8*L, 2*L, 8*L, 6*L);
    qp.drawLine(8*L, 10*L, 8*L, 14*L);
    qp.drawLine(2*L, 8*L, 6*L, 8*L);
    qp.drawLine(10*L, 8*L, 14*L, 8*L);

    for (int i = 0; i < 24; ++i) {
        if (mill->getField(i) == EMPTY) {
        }
        if (mill->getField(i) == WHITE) {
            qp.setBrush(Qt::white);
            qp.drawEllipse(coord[i*3+1]*L-L/2, coord[i*3+2]*L-L/2, L, L);
        }
        if (mill->getField(i) == BLACK) {
            qp.setBrush(Qt::black);
            qp.drawEllipse(coord[i*3+1]*L-L/2, coord[i*3+2]*L-L/2, L, L);
        }
    }
}
