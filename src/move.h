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

    bool operator==(const Move &other) const {
        return x == other.x &&
            y == other.y &&
            z == other.z &&
            length == other.length &&
            capture == other.capture;
    }

    std::string toString() const;
    static Move getMove(QString);
    static int getIdx(int, int);
};
