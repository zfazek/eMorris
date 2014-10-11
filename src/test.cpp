#include "mill.h"
#include <assert.h>
#include <stdio.h>
#include <QString>
#include <iostream>

using namespace std;

void make_test_move_check(Mill *mill, QString move, int expected) {
    int result;
    result = mill->moveCheck(move, true);
    //printf("Test: %s -> %d\n", move.toStdString().c_str(), result);
    assert(result == expected);
}

void make_test_is_end(Mill *mill, int expected) {
    int result;
    result = mill->isEnd();
    //printf("Test: %d\n", result);
    assert(result == expected);
}

void test_moveXX() {
    Mill *mill = new Mill();
    make_test_move_check(mill, "d6", -1);
    make_test_move_check(mill, "moved6", -1);
    make_test_move_check(mill, "mov d6", -1);
    make_test_move_check(mill, "move  d2", -1);
    make_test_move_check(mill, "move d0", -1);
    make_test_move_check(mill, "move d8", -1);
    make_test_move_check(mill, "move h4", -1);
    make_test_move_check(mill, "move h8", -1);
    make_test_move_check(mill, "move d2 ", -1);
    make_test_move_check(mill, "move d2", 0);
    make_test_move_check(mill, "move d2d4", -1);
    make_test_move_check(mill, "move d6", 0);
    make_test_move_check(mill, "move d6", -1);
    {
        int a[] = {0,    1,    1,
            0,  2,  0,
            0,0,0,
            0,0,0,  2,2,2,
            0,0,0,
            0,  0,  0,
            0,    0,    0};
        mill->setPos(a, 6, 6, true);
        make_test_move_check(mill, "move a7", -1);
        make_test_move_check(mill, "move a7g1", -1);
        make_test_move_check(mill, "move a7g4", -1);
        make_test_move_check(mill, "move a7a7", -1);
        make_test_move_check(mill, "move a7g7", -1);
        make_test_move_check(mill, "move a7g4g1", -1);
        make_test_move_check(mill, "move a7d6", 0);
    }
    {
        int a[] = {0,    2,    2,
            0,  1,  0,
            0,0,0,
            0,0,0,  1,1,1,
            0,0,0,
            0,  0,  0,
            0,    0,    0};
        mill->setPos(a, 6, 6, false);
        make_test_move_check(mill, "move a7", -1);
        make_test_move_check(mill, "move a7g1", -1);
        make_test_move_check(mill, "move a7g4", -1);
        make_test_move_check(mill, "move a7a7", -1);
        make_test_move_check(mill, "move a7g7", -1);
        make_test_move_check(mill, "move a7g4g1", -1);
        make_test_move_check(mill, "move a7d6", 0);
    }
    {
        int a[] = {0,    1,    1,
            0,  0,  0,
            0,0,0,
            0,0,0,  2,2,2,
            0,0,0,
            0,  0,  0,
            0,    0,    0};
        mill->setPos(a, 6, 6, true);
        make_test_move_check(mill, "move a7g4", 0);
    }
    {
        int a[] = {0,    2,    2,
            0,  0,  0,
            0,0,0,
            0,0,0,  1,1,1,
            0,0,0,
            0,  0,  0,
            0,    0,    0};
        mill->setPos(a, 6, 6, false);
        make_test_move_check(mill, "move a7g4", 0);
    }
    {
        int a[] = {0,    2,    2,
            0,  0,  1,
            0,0,0,
            0,0,0,  1,1,1,
            0,0,0,
            0,  0,  1,
            0,    0,    0};
        mill->setPos(a, 6, 6, false);
        make_test_move_check(mill, "move a7f4", 0);
    }
    {
        int a[] = {0,    1,    1,
            0,  0,  0,
            0,0,0,
            0,0,0,  2,2,2,
            0,0,0,
            0,  0,  0,
            0,    0,    1};
        mill->setPos(a, 0, 0, true);
        make_test_move_check(mill, "move g1a1", 0);
    }
    {
        int a[] = {0,    1,    1,
            0,  0,  0,
            0,0,0,
            0,0,0,  2,2,2,
            0,0,0,
            0,  0,  0,
            0,    0,    1};
        mill->setPos(a, 0, 0, true);
        make_test_move_check(mill, "move g1d1", 0);
    }
    {
        int a[] = {0,    1,    1,
            0,  0,  0,
            0,0,0,
            0,0,0,  2,2,2,
            0,0,0,
            0,  0,  0,
            0,    0,    1};
        mill->setPos(a, 0, 0, true);
        make_test_move_check(mill, "move g1g4", -1);
    }
    {
        int a[] = {0,    1,    1,
            0,  0,  0,
            0,0,0,
            0,0,0,  2,2,2,
            0,0,0,
            0,  0,  0,
            0,    0,    1};
        mill->setPos(a, 0, 0, true);
        make_test_move_check(mill, "move g1a7", -1);
    }
    {
        int a[] = {0,    1,    1,
            0,  0,  0,
            0,0,0,
            0,0,0,  2,2,2,
            0,0,0,
            0,  0,  0,
            0,    0,    1};
        mill->setPos(a, 0, 0, true);
        make_test_move_check(mill, "move g1a7f6", -1);
    }
    {
        int a[] = {0,    1,    1,
            0,  0,  0,
            0,0,0,
            0,0,0,  2,2,2,
            0,0,0,
            0,  0,  0,
            0,    0,    1};
        mill->setPos(a, 0, 0, true);
        make_test_move_check(mill, "move g1a7d7", -1);
    }
    {
        int a[] = {0,    1,    1,
            0,  0,  0,
            0,0,0,
            0,0,0,  2,2,2,
            0,0,0,
            0,  0,  0,
            0,    0,    1};
        mill->setPos(a, 0, 0, true);
        make_test_move_check(mill, "move g1a7f4", 0);
    }
    {
        int a[] = {0,    1,    1,
            0,  0,  0,
            0,0,0,
            2,0,0,  2,2,2,
            0,0,0,
            0,  0,  0,
            0,    0,    1};
        mill->setPos(a, 0, 0, true);
        make_test_move_check(mill, "move g1a7f4", -1);
    }
    {
        int a[] = {0,    1,    1,
            0,  0,  0,
            0,0,0,
            2,0,0,  2,2,2,
            0,0,0,
            0,  0,  0,
            0,    0,    1};
        mill->setPos(a, 0, 0, true);
        make_test_move_check(mill, "move g1a7b4", -1);
    }
    {
        int a[] = {0,    1,    1,
            1,  0,  0,
            0,0,0,
            2,0,0,  2,2,2,
            0,0,0,
            0,  0,  0,
            0,    0,    1};
        mill->setPos(a, 0, 0, true);
        make_test_move_check(mill, "move g1a7a4", -1);
    }
    {
        int a[] = {0,    1,    1,
            0,  0,  0,
            0,0,0,
            2,0,0,  2,2,2,
            0,0,0,
            0,  0,  0,
            0,    0,    1};
        mill->setPos(a, 0, 0, true);
        make_test_move_check(mill, "move g1a7a4", 0);
    }
    {
        int a[] = {0,    2,    2,
            0,  0,  0,
            0,0,0,
            0,0,0,  1,1,1,
            0,0,0,
            0,  0,  0,
            0,    0,    2};
        mill->setPos(a, 0, 0, false);
        make_test_move_check(mill, "move g1a1", 0);
    }
    {
        int a[] = {0,    2,    2,
            0,  0,  0,
            0,0,0,
            0,0,0,  1,1,1,
            0,0,0,
            0,  0,  0,
            0,    0,    2};
        mill->setPos(a, 0, 0, false);
        make_test_move_check(mill, "move g1d1", 0);
    }
    {
        int a[] = {0,    2,    2,
            0,  0,  0,
            0,0,0,
            0,0,0,  1,1,1,
            0,0,0,
            0,  0,  0,
            0,    0,    2};
        mill->setPos(a, 0, 0, false);
        make_test_move_check(mill, "move g1g4", -1);
    }
    {
        int a[] = {0,    2,    2,
            0,  0,  0,
            0,0,0,
            0,0,0,  1,1,1,
            0,0,0,
            0,  0,  0,
            0,    0,    2};
        mill->setPos(a, 0, 0, false);
        make_test_move_check(mill, "move g1a7", -1);
    }
    {
        int a[] = {0,    2,    2,
            0,  0,  0,
            0,0,0,
            0,0,0,  1,1,1,
            0,0,0,
            0,  0,  0,
            0,    0,    2};
        mill->setPos(a, 0, 0, false);
        make_test_move_check(mill, "move g1a7f6", -1);
    }
    {
        int a[] = {0,    2,    2,
            0,  0,  0,
            0,0,0,
            0,0,0,  1,1,1,
            0,0,0,
            0,  0,  0,
            0,    0,    2};
        mill->setPos(a, 0, 0, false);
        make_test_move_check(mill, "move g1a7d7", -1);
    }
    {
        int a[] = {0,    2,    2,
            0,  0,  0,
            0,0,0,
            0,0,0,  1,1,1,
            0,0,0,
            0,  0,  0,
            0,    0,    2};
        mill->setPos(a, 0, 0, false);
        make_test_move_check(mill, "move g1a7f4", 0);
    }
    {
        int a[] = {0,    2,    2,
            0,  0,  0,
            0,0,0,
            1,0,0,  1,1,1,
            0,0,0,
            0,  0,  0,
            0,    0,    2};
        mill->setPos(a, 0, 0, false);
        make_test_move_check(mill, "move g1a7f4", -1);
    }
    {
        int a[] = {0,    2,    2,
            0,  0,  0,
            0,0,0,
            1,0,0,  1,1,1,
            0,0,0,
            0,  0,  0,
            0,    0,    2};
        mill->setPos(a, 0, 0, false);
        make_test_move_check(mill, "move g1a7b4", -1);
    }
    {
        int a[] = {0,    2,    2,
            0,  0,  0,
            0,0,0,
            1,0,0,  1,1,1,
            0,0,0,
            0,  0,  0,
            0,    0,    2};
        mill->setPos(a, 0, 0, false);
        make_test_move_check(mill, "move g1a7a4", 0);
    }
    {
        int a[] = {0,    2,    2,
            2,  0,  0,
            0,0,0,
            1,0,0,  1,1,1,
            0,0,0,
            0,  0,  0,
            0,    0,    2};
        mill->setPos(a, 0, 0, false);
        make_test_move_check(mill, "move g1a7a4", -1);
    }
    {
        int a[] = {0,    0,    0,
            0,  2,  2,
            2,0,2,
            0,0,0,  0,0,0,
            0,0,0,
            1,  1,  1,
            1,    0,    1};
        mill->setPos(a, 0, 0, true);
        make_test_move_check(mill, "move d2d1d6", 0);
    }
    {
        int a[] = {0,0,0,
            2,0,0,
            0,2,0,
            0,1,0,  0,2,0,
            0,0,0,
            1,1,0,
            0,0,0};
        mill->setPos(a, 6, 6, true);
        make_test_move_check(mill, "move b6b6", -1);
    }
    {
        int a[] = {0,2,0,
            2,0,0,
            0,0,0,
            0,2,1,  0,0,2,
            1,2,0,
            2,2,0,
            0,2,1};
        mill->setPos(a, 0, 0, true);
        make_test_move_check(mill, "move c3g7d7", -1);
    }
}

void test_isEnd() {
    Mill *mill = new Mill();
    {
        int a[] = {0,    2,    2,
            0,  0,  0,
            0,0,0,
            1,0,0,  1,1,1,
            0,0,0,
            0,  0,  0,
            0,    0,    2};
        mill->setPos(a, 0, 0, true);
        make_test_is_end(mill, 0);
    }
    {
        int a[] = {0,    2,    2,
            0,  0,  0,
            0,0,0,
            1,0,0,  1,1,1,
            0,0,0,
            0,  0,  0,
            0,    0,    0};
        mill->setPos(a, 0, 0, true);
        make_test_is_end(mill, 1);
    }
    {
        int a[] = {0,    2,    2,
            0,  0,  0,
            0,0,0,
            1,0,0,  1,1,1,
            0,0,0,
            0,  0,  0,
            0,    0,    0};
        mill->setPos(a, 0, 1, true);
        make_test_is_end(mill, 0);
    }
    {
        int a[] = {0,    1,    1,
            0,  0,  0,
            0,0,0,
            2,0,0,  2,2,2,
            0,0,0,
            0,  0,  0,
            0,    0,    1};
        mill->setPos(a, 0, 0, true);
        make_test_is_end(mill, 0);
    }
    {
        int a[] = {0,    1,    1,
            0,  0,  0,
            0,0,0,
            2,0,0,  2,2,2,
            0,0,0,
            0,  0,  0,
            0,    0,    0};
        mill->setPos(a, 0, 0, true);
        make_test_is_end(mill, -1);
    }
    {
        int a[] = {0,    1,    1,
            0,  0,  0,
            0,0,0,
            2,0,0,  2,2,2,
            0,0,0,
            0,  0,  0,
            0,    0,    0};
        mill->setPos(a, 1, 0, true);
        make_test_is_end(mill, 0);
    }
    {
        int a[] = {1,    1,    1,
            2,  2,  2,
            2,1,0,
            0,1,1,  0,1,0,
            0,0,0,
            1,  1,  1,
            1,    0,    1};
        mill->setPos(a, 0, 0, false);
        make_test_is_end(mill, 1);
    }
    {
        int a[] = {1,    1,    1,
            2,  2,  2,
            0,1,0,
            0,1,1,  0,1,0,
            0,0,0,
            1,  1,  1,
            1,    0,    1};
        mill->setPos(a, 0, 0, false);
        make_test_is_end(mill, 0);
    }
    {
        int a[] = {2,    2,    2,
            1,  1,  1,
            1,2,0,
            0,2,2,  0,2,0,
            0,0,0,
            2,  2,  2,
            2,    0,    2};
        mill->setPos(a, 0, 0, true);
        make_test_is_end(mill, -1);
    }
    {
        int a[] = {2,    2,    2,
            1,  1,  1,
            0,2,0,
            0,2,2,  0,2,0,
            0,0,0,
            2,  2,  2,
            2,    0,    2};
        mill->setPos(a, 0, 0, true);
        make_test_is_end(mill, 0);
    }
}

int main() {
    test_moveXX();
    test_isEnd();
    return 0;
}
