#include "move.h"
#include <QString>

using namespace std;

/******************************************************************************
 *
 * idx of field, y, x coordinates
 * 0, 0 is the top left corner
 *
 ******************************************************************************/
int coordHelper[72] = {
    0, 0, 0,
    1, 0, 3,
    2, 0, 6,
    3, 1, 1,
    4, 1, 3,
    5, 1, 5,
    6, 2, 2,
    7, 2, 3,
    8, 2, 4,
    9, 3, 0,
    10, 3, 1,
    11, 3, 2,
    12, 3, 4,
    13, 3, 5,
    14, 3, 6,
    15, 4, 2,
    16, 4, 3,
    17, 4, 4,
    18, 5, 1,
    19, 5, 3,
    20, 5, 5,
    21, 6, 0,
    22, 6, 3,
    23, 6, 6
};

Move::Move() {
    x = 0;
    y = 0;
    z = 0;
    capture = false;
    length = 0;
}

Move::Move(int l, bool cap, int a) : Move() {
    length = l;
    capture = cap;
    x = a;
}

Move::Move(int l, bool cap, int a, int b) : Move(l, cap, a) {
    y = b;
}

Move::Move(int l, bool cap, int a, int b, int c) : Move(l, cap, a, b) {
    z = c;
}

Move::~Move() {
}

/******************************************************************************
 *
 * Gets the index of the field from coordinates
 *
 ******************************************************************************/
int Move::getIdx(int x, int y) {
    for (int i = 0; i < 24; ++i) {
        if (coordHelper[i * 3 + 1] == y &&
                coordHelper[i * 3 + 2] == x)
            return i;
    }
    return -1;
}

Move Move::getMove(QString input) {
    Move move;
    move.length = 0;
    int length = input.length();

    /* move d2 */
    if (length == 7) {
        int x = input.at(5).toAscii() - 'a';
        int y = 7 - input.at(6).toAscii() + '0';
        int i = getIdx(x, y);
        if (i == -1) return move;
        move.length = 1;
        move.capture = false;
        move.x = i;
        return move;
    }

    /* move d2d3 */
    if (length == 9) {
        int x1 = input.at(5).toAscii() - 'a';
        int y1 = 7 - input.at(6).toAscii() + '0';
        int i1 = getIdx(x1, y1);
        if (i1 == -1) return move;
        int x2 = input.at(7).toAscii() - 'a';
        int y2 = 7 - input.at(8).toAscii() + '0';
        int i2 = getIdx(x2, y2);
        if (i2 == -1) return move;
        move.length = 2;
        move.capture = false;
        move.x = i1;
        move.y = i2;
        return move;
    }

    /* move d2,d3 */
    if (length == 10) {
        int x1 = input.at(5).toAscii() - 'a';
        int y1 = 7 - input.at(6).toAscii() + '0';
        int i1 = getIdx(x1, y1);
        if (i1 == -1) return move;
        int x2 = input.at(8).toAscii() - 'a';
        int y2 = 7 - input.at(9).toAscii() + '0';
        int i2 = getIdx(x2, y2);
        if (i2 == -1) return move;
        move.length = 2;
        move.capture = true;
        move.x = i1;
        move.y = i2;
        return move;
    }

    /* move a1d1,f4 */
    if (length == 12) {
        int x1 = input.at(5).toAscii() - 'a';
        int y1 = 7 - input.at(6).toAscii() + '0';
        int i1 = getIdx(x1, y1);
        if (i1 == -1) return move;
        int x2 = input.at(7).toAscii() - 'a';
        int y2 = 7 - input.at(8).toAscii() + '0';
        int i2 = getIdx(x2, y2);
        if (i2 == -1) return move;
        int comma = input.at(9).toAscii();
        if (comma != ',') return move;
        int x3 = input.at(10).toAscii() - 'a';
        int y3 = 7 - input.at(11).toAscii() + '0';
        int i3 = getIdx(x3, y3);
        if (i3 == -1) return move;
        move.length = 3;
        move.capture = true;
        move.x = i1;
        move.y = i2;
        move.z = i3;
        return move;
    }
    return move;
}

string Move::toString() const {
    if (length == 1) {
        char c = coordHelper[x * 3 + 2] + 'a';
        return "move " + string(1, c) + to_string(7 - coordHelper[x * 3 + 1]);
    } else if (length == 2) {
        char c1 = coordHelper[x * 3 + 2] + 'a';
        char c2 = coordHelper[y * 3 + 2] + 'a';
        if (capture) {
            return "move " + string(1, c1) + to_string(7 - coordHelper[x * 3 + 1]) + "," +
                string(1, c2) + to_string(7 - coordHelper[y * 3 + 1]);
        } else {
            return "move " + string(1, c1) + to_string(7 - coordHelper[x * 3 + 1]) +
                string(1, c2) + to_string(7 - coordHelper[y * 3 + 1]);
        }
    } else {
        char c1 = coordHelper[x * 3 + 2] + 'a';
        char c2 = coordHelper[y * 3 + 2] + 'a';
        char c3 = coordHelper[z * 3 + 2] + 'a';
        if (capture) {
            return "move " + string(1, c1) + to_string(7 - coordHelper[x * 3 + 1]) +
                string(1, c2) + to_string(7 - coordHelper[y * 3 + 1]) + "," + 
                string(1, c3) + to_string(7 - coordHelper[z * 3 + 1]);
        } else {
            return "move " + string(1, c1) + to_string(7 - coordHelper[x * 3 + 1]) +
                string(1, c2) + to_string(7 - coordHelper[y * 3 + 1]) + 
                string(1, c3) + to_string(7 - coordHelper[z * 3 + 1]);
        }
    }
}
