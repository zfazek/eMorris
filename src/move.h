#pragma once

#include <string>

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
};
