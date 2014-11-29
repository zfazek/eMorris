#include <stdio.h>
#include "table.h"
#include "move.h"
#include <vector>

using namespace std;

/******************************************************************************
 *
 * idx of field, next two values are the mill1 buddies,
 * last two values are the mill2 buddies
 * It is just a helper array, not used during the game
 *
 ******************************************************************************/
int millHelper[] = {
    0, 1, 2, 9, 21,
    1, 0, 2, 4, 7,
    2, 0, 1, 14, 23,
    3, 4, 5, 10, 18,
    4, 1, 7, 3, 5,
    5, 3, 4, 13, 20,
    6, 7, 8, 11, 15,
    7, 1, 4, 6, 8,
    8, 6, 7, 12, 17,
    9, 10, 11, 0, 21,
    10, 9, 11, 3, 18,
    11, 6, 15, 9, 10,
    12, 8, 17, 13, 14,
    13, 5, 20, 12, 14,
    14, 2, 23, 12, 13,
    15, 6, 11, 16, 17,
    16, 15, 17, 19, 22,
    17, 8, 12, 15, 16,
    18, 3, 10, 19, 20,
    19, 16, 22, 18, 20,
    20, 5, 13, 18, 19,
    21, 0, 9, 22, 23,
    22, 16, 19, 21, 23,
    23, 2, 14, 21, 22
};

/******************************************************************************
 *
 * idx of field, neighbor indices
 *
 ******************************************************************************/
int millNeighbors[] = {
    0, 1, 1, 9, 9,
    1, 0, 2, 4, 4,
    2, 1, 1, 14, 14,
    3, 4, 4, 10, 10,
    4, 1, 3, 5, 7,
    5, 4, 4, 13, 13,
    6, 7, 7, 11, 11,
    7, 4, 6, 8, 8,
    8, 7, 7, 12, 12,
    9, 0, 10, 21, 21,
    10, 3, 9, 11, 18,
    11, 6, 10, 15, 15,
    12, 8, 13, 17, 17,
    13, 5, 12, 14, 20,
    14, 2, 13, 23, 23,
    15, 11, 11, 16, 16,
    16, 15, 17, 19, 19,
    17, 12, 12, 16, 16,
    18, 10, 10, 19, 19,
    19, 16, 18, 20, 22,
    20, 13, 13, 19, 19,
    21, 9, 9, 22, 22,
    22, 19, 21, 23, 23,
    23, 14, 14, 22, 22
};

Table::Table() {
    initRules();
}

Table::Table(const Table *t) : Table() {
    data.whiteToMove = t->data.whiteToMove;
    data.whiteHand = t->data.whiteHand;
    data.blackHand = t->data.blackHand;
    for (int i = 0; i < 24; i++) {
        data.table[i] = t->data.table[i];
    }
}

Table::~Table() {
}

int Table::getWhiteHand() {
    return data.whiteHand;
}

int Table::getBlackHand() {
    return data.blackHand;
}

void Table::initTable() {
    data.whiteHand = 9;
    data.blackHand = 9;
    for (int i = 0; i < 24; ++i) {
        data.table[i] = EMPTY;
    }
    data.whiteToMove = true;
}

/******************************************************************************
 *
 * These values are not changed during the game
 * mill contains the possible mills for a field
 *
 ******************************************************************************/
void Table::initRules() {
    int i = 0;
    int j;
    while (i < 120) {
        j = millHelper[i++];
        mill[j][0] = millHelper[i++];
        mill[j][1] = millHelper[i++];
        mill[j][2] = millHelper[i++];
        mill[j][3] = millHelper[i++];
    }
}

int Table::moveCheck(Move move, bool updateHistory) {
    if (move.length == 1) {
        return moveCheck(move.x, updateHistory);
    } else if (move.length == 2) {
        int n_white = getNofPiece(WHITE);
        int n_black = getNofPiece(BLACK);
        return moveCheck(move.x, move.y, updateHistory, n_white, n_black);
    } else {
        int n_white = getNofPiece(WHITE);
        int n_black = getNofPiece(BLACK);
        return moveCheck(move.x, move.y, move.z, updateHistory, n_white, n_black);
    }
}

/******************************************************************************
 *
 * Checks if the move is legal.
 * Only one index is given.
 * It also makes the move in the table if makeMove is true.
 * Otherwise it only checks if it is legal or not.
 *
 ******************************************************************************/
int Table::moveCheck(int i, bool makeMove) {
    if (data.whiteToMove and data.whiteHand == 0) return -1;
    if (! data.whiteToMove and data.blackHand == 0) return -1;
    if (data.table[i] != EMPTY) return -1;
    if (data.whiteToMove) {
        if (isMill(i, WHITE)) return -1;
        if (makeMove) {
            data.table[i] = WHITE;
            data.whiteToMove = false;
            --data.whiteHand;
        }
        return 0;
    } else {
        if (isMill(i, BLACK)) return -1;
        if (makeMove) {
            data.table[i] = BLACK;
            data.whiteToMove = true;
            --data.blackHand;
        }
        return 0;
    }
    return 0;
}

/******************************************************************************
 *
 * Checks if the move is legal.
 * Only two indices are given.
 * It also makes the move in the table if makeMove is true.
 * Otherwise it only checks if it is legal or not.
 *
 ******************************************************************************/
int Table::moveCheck(int i1, int i2, bool makeMove, int n_white, int n_black) {

    /* Simple WHITE move */
    if (data.whiteToMove && data.whiteHand == 0) {
        if (data.table[i1] != WHITE) return -1;
        if (data.table[i2] != EMPTY) return -1;
        if (n_white > 3 && ! isNeighbor(i1, i2)) return -1;
        data.table[i1] = EMPTY;
        data.table[i2] = WHITE;

        /* Move into mill but there is no pick */
        if (isMill(i2, WHITE)) {
            data.table[i1] = WHITE;
            data.table[i2] = EMPTY;
            return -1;
        }
        if (makeMove) {
            data.whiteToMove = false;
        } else {
            data.table[i1] = WHITE;
            data.table[i2] = EMPTY;
        }
        return 0;
    }

    /* Simple BLACK move */
    if (!data.whiteToMove && data.blackHand == 0) {
        if (data.table[i1] != BLACK) return -1;
        if (data.table[i2] != EMPTY) return -1;
        if (n_black > 3 && ! isNeighbor(i1, i2)) return -1;
        data.table[i1] = EMPTY;
        data.table[i2] = BLACK;

        /* Move into mill but there is no pick */
        if (isMill(i2, BLACK)) {
            data.table[i1] = BLACK;
            data.table[i2] = EMPTY;
            return -1;
        }
        if (makeMove) {
            data.whiteToMove = true;
        } else {
            data.table[i1] = BLACK;
            data.table[i2] = EMPTY;
        }
        return 0;
    }

    /* Mill from hand */
    if (data.whiteToMove && data.whiteHand > 0) {
        if (data.table[i1] != EMPTY) return -1;
        if (data.table[i2] != BLACK) return -1;
        if (isMill(i1, WHITE)) {

            /* You can not pick from mill */
            if (isMill(i2, BLACK) && hasSoloMorris(BLACK)) return -1;
            if (makeMove) {
                data.table[i1] = WHITE;
                data.table[i2] = EMPTY;
                data.whiteToMove = false;
                --data.whiteHand;
            }
            return 0;
        }
    }
    if (!data.whiteToMove && data.blackHand > 0) {
        if (data.table[i1] != EMPTY) return -1;
        if (data.table[i2] != WHITE) return -1;
        if (isMill(i1, BLACK)) {

            /* You can not pick from mill */
            if (isMill(i2, WHITE) && hasSoloMorris(WHITE)) return -1;
            if (makeMove) {
                data.table[i1] = BLACK;
                data.table[i2] = EMPTY;
                data.whiteToMove = true;
                --data.blackHand;
            }
            return 0;
        }
    }
    return -1;
}

/******************************************************************************
 *
 * Checks if the move is legal.
 * Only two indices are given.
 * It also makes the move in the table if makeMove is true.
 * Otherwise it only checks if it is legal or not.
 *
 ******************************************************************************/
int Table::moveCheck(int i1, int i2, int i3, bool makeMove, int n_white, int n_black) {
    if (data.whiteToMove && data.whiteHand == 0) {
        if (data.table[i1] != WHITE) return -1;
        if (data.table[i2] != EMPTY) return -1;
        if (data.table[i3] != BLACK) return -1;
        if (n_white > 3 && ! isNeighbor(i1, i2)) return -1;
        data.table[i1] = EMPTY;
        data.table[i2] = WHITE;
        if (!isMill(i2, WHITE)) {
            data.table[i1] = WHITE;
            data.table[i2] = EMPTY;
            return -1;
        }
        if (isMill(i3, BLACK) && hasSoloMorris(BLACK)) {
            data.table[i1] = WHITE;
            data.table[i2] = EMPTY;
            return -1;
        }
        if (makeMove) {
            data.table[i3] = EMPTY;
            data.whiteToMove = false;
        } else {
            data.table[i1] = WHITE;
            data.table[i2] = EMPTY;
        }
        return 0;
    }

    if (! data.whiteToMove && data.blackHand == 0) {
        if (data.table[i1] != BLACK) return -1;
        if (data.table[i2] != EMPTY) return -1;
        if (data.table[i3] != WHITE) return -1;
        if (n_black > 3 && ! isNeighbor(i1, i2)) return -1;
        data.table[i1] = EMPTY;
        data.table[i2] = BLACK;
        if (!isMill(i2, BLACK)) {
            data.table[i1] = BLACK;
            data.table[i2] = EMPTY;
            return -1;
        }
        if (isMill(i3, WHITE) && hasSoloMorris(WHITE)) {
            data.table[i1] = BLACK;
            data.table[i2] = EMPTY;
            return -1;
        }
        if (makeMove) {
            data.table[i3] = EMPTY;
            data.whiteToMove = true;
        } else {
            data.table[i1] = BLACK;
            data.table[i2] = EMPTY;
        }
        return 0;
    }
    return -1;
}

/******************************************************************************
 *
 * Returns if it is mill or not
 *
 ******************************************************************************/
bool Table::isMill(int idx, int color) {
    if (data.table[mill[idx][0]] == color &&
            data.table[mill[idx][1]] == color) return true;
    if (data.table[mill[idx][2]] == color &&
            data.table[mill[idx][3]] == color) return true;
    return false;
}

/******************************************************************************
 *
 * Returns the number of men of a color
 *
 ******************************************************************************/
int Table::getNofPiece(int color) {
    int n = 0;
    for (int i = 0; i < 24; i++) {
        if (data.table[i] == color) {
            n++;
        }
    }
    return n;
}

/******************************************************************************
 *
 * Returns all the legal moves.
 * It uses brute force to generate all the moves and checks if it is legal
 * one by one.
 *
 ******************************************************************************/
vector<Move> Table::getAllMoves() {
    vector<Move> moves;
    int n_white = getNofPiece(WHITE);
    int n_black = getNofPiece(BLACK);
    if (data.whiteToMove) {
        if (data.whiteHand > 0) {
            for (int i = 0; i < 24; i++) {
                if (moveCheck(i, false) == 0) {
                    Move move(1, false, i);
                    moves.push_back(move);
                } else {
                    for (int j = 0; j < 24; j++) {
                        if (moveCheck(i, j, false, n_white, n_black) == 0) {
                            Move move(2, true, i, j);
                            moves.push_back(move);
                        }
                    }
                }
            }
        } else {
            for (int i = 0; i < 24; i++) {
                for (int j = 0; j < 24; j++) {
                    if (moveCheck(i, j, false, n_white, n_black) == 0) {
                        Move move(2, false, i, j);
                        moves.push_back(move);
                    } else {
                        for (int k = 0; k < 24; k++) {
                            if (moveCheck(i, j, k, false, n_white, n_black) == 0) {
                                Move move(3, true, i, j, k);
                                moves.push_back(move);
                            }
                        }
                    }
                }
            }
        }
    } else {
        if (data.blackHand > 0) {
            for (int i = 0; i < 24; i++) {
                if (moveCheck(i, false) == 0) {
                    Move move(1, false, i);
                    moves.push_back(move);
                } else {
                    for (int j = 0; j < 24; j++) {
                        if (moveCheck(i, j, false, n_white, n_black) == 0) {
                            Move move(2, true, i, j);
                            moves.push_back(move);
                        }
                    }
                }
            }
        } else {
            for (int i = 0; i < 24; i++) {
                for (int j = 0; j < 24; j++) {
                    if (moveCheck(i, j, false, n_white, n_black) == 0) {
                        Move move(2, false, i, j);
                        moves.push_back(move);
                    } else {
                        for (int k = 0; k < 24; k++) {
                            if (moveCheck(i, j, k, false, n_white, n_black) == 0) {
                                Move move(3, true, i, j, k);
                                moves.push_back(move);
                            }
                        }
                    }
                }
            }
        }
    }
    return moves;
}

int Table::getField(int i) {
    return data.table[i];
}

/******************************************************************************
 *
 * Returns 0 if not end, 1 if white won, -1 if black won
 *
 ******************************************************************************/
int Table::isEnd() {
    if (data.whiteHand > 0) return 0;
    if (data.blackHand > 0) return 0;
    if (getNofPiece(WHITE) < 3) return -1;
    if (getNofPiece(BLACK) < 3) return 1;
    int n_moves = getAllMoves().size();
    if (data.whiteToMove && n_moves == 0) return -1;
    if (! data.whiteToMove && n_moves == 0) return 1;
    return 0;
}

/******************************************************************************
 *
 * Returns true if idx1 and idx2 are neighbours, otherwise false
 *
 ******************************************************************************/
bool Table::isNeighbor(int idx1, int idx2) {
    return millNeighbors[idx1 * 5 + 1] == idx2 ||
        millNeighbors[idx1 * 5 + 2] == idx2 ||
        millNeighbors[idx1 * 5 + 3] == idx2 ||
        millNeighbors[idx1 * 5 + 4] == idx2;
}

/******************************************************************************
 *
 * Checks if the user has any man in no mill.
 * It needs to check that picking from mill is legal or not.
 * If hasSoloMorris(color) is false it is allowed to pick from mill.
 *
 ******************************************************************************/
bool Table::hasSoloMorris(int color) {
    for (int i = 0; i < 24; i++)
        if (data.table[i] == color)
            if (! isMill(i, color))
                return true;
    return false;
}

/******************************************************************************
 *
 * Sets any position. It is good for testing.
 * t contains the table
 *
 ******************************************************************************/
void Table::setPos(int *t, int w, int b, bool wToMove) {
    for (int i = 0; i < 24; i++)
        data.table[i] = *(t+i);
    data.whiteHand = w;
    data.blackHand = b;
    data.whiteToMove = wToMove;
}

void Table::backupPosition(const Table* const t) {
    data.whiteToMove = t->data.whiteToMove;
    data.whiteHand = t->data.whiteHand;
    data.blackHand = t->data.blackHand;
    for (int i = 0; i < 24; i++) data.table[i] = t->data.table[i];
}

void Table::restorePosition(Table* const t) const {
    t->data.whiteToMove = data.whiteToMove;
    t->data.whiteHand = data.whiteHand;
    t->data.blackHand = data.blackHand;
    for (int i = 0; i < 24; i++) t->data.table[i] = data.table[i];
}

/******************************************************************************
 *
 * Prints the table to the console. Only for testing.
 *
 ******************************************************************************/
void Table::printTable() {
    int *t = data.table;
    printf("%s\n", data.whiteToMove ? "White to move" : "Black to move");
    printf("7 %d-----%d-----%d Black hand: %d\n", *t, *(t+1), *(t+2), data.blackHand);
    printf("  |     |     |\n");
    printf("6 | %d---%d---%d |\n", *(t+3), *(t+4), *(t+5));
    printf("  | |   |   | |\n");
    printf("5 | | %d-%d-%d | |\n", *(t+6), *(t+7), *(t+8));
    printf("  | | |   | | |\n");
    printf("4 %d-%d-%d   %d-%d-%d\n", *(t+9), *(t+10), *(t+11), *(t+12), *(t+13), *(t+14));
    printf("  | | |   | | |\n");
    printf("3 | | %d-%d-%d | |\n", *(t+15), *(t+16), *(t+17));
    printf("  | |   |   | |\n");
    printf("2 | %d---%d---%d |\n", *(t+18), *(t+19), *(t+20));
    printf("  |     |     |\n");
    printf("1 %d-----%d-----%d White hand: %d\n", *(t+21), *(t+22), *(t+23), data.whiteHand);
    printf("  A B C D E F G\n");
}

TableData Table::getTableData() {
    return data;
}
