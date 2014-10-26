#pragma once

#include <string>

class QString;

extern int coordHelper[72];

struct Move {
    int length;
    bool capture;
    int x;
    int y;
    int z;

    Move();
    Move(int length, bool capture, int x);
    Move(int length, bool capture, int x, int y);
    Move(int length, bool capture, int x, int y, int z);
    ~Move();
    std::string toString();
    static Move getMove(QString);
    static int getIdx(int, int);
};
