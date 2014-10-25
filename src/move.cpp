#include "move.h"

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

Move::Move(int l, bool cap, int a) {
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

string Move::toString() {
    if (length == 1) {
        char c = coordHelper[x * 3 + 1] + 'a';
        return "move " + string(1, c) + to_string(7 - coordHelper[x * 3 + 2]);
    } else if (length == 2) {
        char c1 = coordHelper[x * 3 + 1] + 'a';
        char c2 = coordHelper[y * 3 + 1] + 'a';
        if (capture) {
            return "move " + string(1, c1) + to_string(7 - coordHelper[x * 3 + 2]) + "," +
                string(1, c2) + to_string(7 - coordHelper[y * 3 + 2]);
        } else {
            return "move " + string(1, c1) + to_string(7 - coordHelper[x * 3 + 2]) +
                string(1, c2) + to_string(7 - coordHelper[y * 3 + 2]);
        }
    } else {
        char c1 = coordHelper[x * 3 + 1] + 'a';
        char c2 = coordHelper[y * 3 + 1] + 'a';
        char c3 = coordHelper[z * 3 + 1] + 'a';
        if (capture) {
            return "move " + string(1, c1) + to_string(7 - coordHelper[x * 3 + 2]) +
                string(1, c2) + to_string(7 - coordHelper[y * 3 + 2]) + "," + 
                string(1, c3) + to_string(7 - coordHelper[z * 3 + 2]);
        } else {
            return "move " + string(1, c1) + to_string(7 - coordHelper[x * 3 + 2]) +
                string(1, c2) + to_string(7 - coordHelper[y * 3 + 2]) + 
                string(1, c3) + to_string(7 - coordHelper[z * 3 + 2]);
        }
    }
}
