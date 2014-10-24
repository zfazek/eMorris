#include "mill.h"
#include "move.h"
#include "table.h"
#include <stdio.h>
#include <iostream>
#include <QString>
#include <vector>
#include <time.h>

using namespace std;


/******************************************************************************
 *
 * idx of field, y, x coordinates
 * 0, 0 is the top left corner
 *
 ******************************************************************************/
int coordHelper[] = {
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

/******************************************************************************
 *
 * idx of field, number of neighbors
 *
 ******************************************************************************/
int nofNeighbors[] = {
    0, 2,
    1, 3,
    2, 2,
    3, 2,
    4, 4,
    5, 2,
    6, 2,
    7, 3,
    8, 2,
    9, 3,
    10, 4,
    11, 3,
    12, 3,
    13, 4,
    14, 3,
    15, 2,
    16, 3,
    17, 2,
    18, 2,
    19, 4,
    20, 2,
    21, 2,
    22, 3,
    23, 2
};

Mill::Mill() {
    table = new Table();
    this->initRules();
    this->initTable(true);
    n = 20;
}

Mill::~Mill() {
}

/******************************************************************************
 *
 * These values are not changed during the game
 * mill contains the possible mills for a field
 *
 ******************************************************************************/
void Mill::initRules() {

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

/******************************************************************************
 *
 * These values should be reset before every new game
 *
 ******************************************************************************/
void Mill::initTable(bool historyClear) {
    table->whiteHand = 9;
    table->blackHand = 9;
    for (int i = 0; i < 24; ++i) {
        table->table[i] = EMPTY;
    }
    if (historyClear) {
        history.clear();
        historyIdx = -1;
    }
    table->whiteToMove = true;
}

int Mill::getCoordX(int i) {
    if (i >= 0 && i <= 23)
        return coordHelper[i * 3 + 2];
    else
        return -1;
}

int Mill::getCoordY(int i) {
    if (i >= 0 && i <= 23)
        return coordHelper[i * 3 + 1];
    else
        return -1;
}

int Mill::getField(int i) {
    return table->table[i];
}

vector<QString> Mill::getHistory() {
    return history;
}

/******************************************************************************
 *
 * Checks if the move is legal.
 * Only one index is given.
 * It also makes the move in the table if makeMove is true.
 * Otherwise it only checks if it is legal or not.
 *
 ******************************************************************************/
int Mill::moveCheck(int i, bool makeMove) {
    if (table->whiteToMove and table->whiteHand == 0) return -1;
    if (!table->whiteToMove and table->blackHand == 0) return -1;
    if (table->table[i] != EMPTY) return -1;
    if (table->whiteToMove) {
        if (isMill(i, WHITE)) return -1;
        if (makeMove) {
            table->table[i] = WHITE;
            table->whiteToMove = false;
            --table->whiteHand;
        }
        return 0;
    } else {
        if (isMill(i, BLACK)) return -1;
        if (makeMove) {
            table->table[i] = BLACK;
            table->whiteToMove = true;
            --table->blackHand;
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
int Mill::moveCheck(int i1, int i2, bool makeMove) {
    int nofWhite = getNofPiece(WHITE);
    int nofBlack = getNofPiece(BLACK);

    /* Simple WHITE move */
    if (table->whiteToMove && table->whiteHand == 0) {
        if (table->table[i1] != WHITE) return -1;
        if (table->table[i2] != EMPTY) return -1;
        if (nofWhite > 3 && ! isNeighbor(i1, i2)) return -1;
        table->table[i1] = EMPTY;
        table->table[i2] = WHITE;

        /* Move into mill but there is no pick */
        if (isMill(i2, WHITE)) {
            table->table[i1] = WHITE;
            table->table[i2] = EMPTY;
            return -1;
        }
        if (makeMove) {
            table->whiteToMove = false;
        } else {
            table->table[i1] = WHITE;
            table->table[i2] = EMPTY;
        }
        return 0;
    }

    /* Simple BLACK move */
    if (!table->whiteToMove && table->blackHand == 0) {
        if (table->table[i1] != BLACK) return -1;
        if (table->table[i2] != EMPTY) return -1;
        if (nofBlack > 3 && ! isNeighbor(i1, i2)) return -1;
        table->table[i1] = EMPTY;
        table->table[i2] = BLACK;

        /* Move into mill but there is no pick */
        if (isMill(i2, BLACK)) {
            table->table[i1] = BLACK;
            table->table[i2] = EMPTY;
            return -1;
        }
        if (makeMove) {
            table->whiteToMove = true;
        } else {
            table->table[i1] = BLACK;
            table->table[i2] = EMPTY;
        }
        return 0;
    }

    /* Mill from hand */
    if (table->whiteToMove && table->whiteHand > 0) {
        if (table->table[i1] != EMPTY) return -1;
        if (isMill(i1, WHITE)) {
            if (table->table[i2] != BLACK) return -1;

            /* You can not pick from mill */
            if (isMill(i2, BLACK) && hasSoloMorris(BLACK)) return -1;
            if (makeMove) {
                table->table[i1] = WHITE;
                table->table[i2] = EMPTY;
                table->whiteToMove = false;
                --table->whiteHand;
            }
            return 0;
        }
    }
    if (!table->whiteToMove && table->blackHand > 0) {
        if (table->table[i1] != EMPTY) return -1;
        if (isMill(i1, BLACK)) {
            if (table->table[i2] != WHITE) return -1;

            /* You can not pick from mill */
            if (isMill(i2, WHITE) && hasSoloMorris(WHITE)) return -1;
            if (makeMove) {
                table->table[i1] = BLACK;
                table->table[i2] = EMPTY;
                table->whiteToMove = true;
                --table->blackHand;
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
int Mill::moveCheck(int i1, int i2, int i3, bool makeMove) {
    if (table->whiteToMove && table->whiteHand > 0) return -1;
    if (!table->whiteToMove && table->blackHand > 0) return -1;
    int nofWhite = getNofPiece(WHITE);
    int nofBlack = getNofPiece(BLACK);

    if (table->whiteToMove && table->whiteHand == 0) {
        if (table->table[i1] != WHITE) return -1;
        if (table->table[i2] != EMPTY) return -1;
        if (nofWhite > 3 && ! isNeighbor(i1, i2)) return -1;
        table->table[i1] = EMPTY;
        table->table[i2] = WHITE;
        if (!isMill(i2, WHITE)) {
            table->table[i1] = WHITE;
            table->table[i2] = EMPTY;
            return -1;
        }
        if (table->table[i3] != BLACK) {
            table->table[i1] = WHITE;
            table->table[i2] = EMPTY;
            return -1;
        }
        if (isMill(i3, BLACK) && hasSoloMorris(BLACK)) {
            table->table[i1] = WHITE;
            table->table[i2] = EMPTY;
            return -1;
        }
        if (makeMove) {
            table->table[i3] = EMPTY;
            table->whiteToMove = false;
        } else {
            table->table[i1] = WHITE;
            table->table[i2] = EMPTY;
        }
        return 0;
    }

    if (!table->whiteToMove && table->blackHand == 0) {
        if (table->table[i1] != BLACK) return -1;
        if (table->table[i2] != EMPTY) return -1;
        if (nofBlack > 3 && ! isNeighbor(i1, i2)) return -1;
        table->table[i1] = EMPTY;
        table->table[i2] = BLACK;
        if (!isMill(i2, BLACK)) {
            table->table[i1] = BLACK;
            table->table[i2] = EMPTY;
            return -1;
        }
        if (table->table[i3] != WHITE) {
            table->table[i1] = BLACK;
            table->table[i2] = EMPTY;
            return -1;
        }
        if (isMill(i3, WHITE) && hasSoloMorris(WHITE)) {
            table->table[i1] = BLACK;
            table->table[i2] = EMPTY;
            return -1;
        }
        if (makeMove) {
            table->table[i3] = EMPTY;
            table->whiteToMove = true;
        } else {
            table->table[i1] = BLACK;
            table->table[i2] = EMPTY;
        }
        return 0;
    }
    return -1;
}




/******************************************************************************
 *
 * Gets the move: "move d2", sets the table, updates variables
 * Returns -1 if illegal move
 * It also makes the move in the table if makeMove is true.
 * Otherwise it only checks if it is legal or not.
 *
 ******************************************************************************/
int Mill::moveCheck(QString input, bool makeMove) {
    int length = input.length();

    /* move d2 */
    if (length == 7) {
        int x = input.at(5).toAscii() - 'a';
        int y = 7 - input.mid(6, 1).toInt();
        int i = getIdx(x, y);
        if (i == -1) return -1;
        return moveCheck(i, makeMove);
    }

    /* move d2d3 */
    if (length == 9) {
        int x1 = input.at(5).toAscii() - 'a';
        int y1 = 7 - input.mid(6, 1).toInt();
        int i1 = getIdx(x1, y1);
        if (i1 == -1) return -1;
        int x2 = input.at(7).toAscii() - 'a';
        int y2 = 7 - input.mid(8, 1).toInt();
        int i2 = getIdx(x2, y2);
        if (i2 == -1) return -1;
        return moveCheck(i1, i2, makeMove);
    }

    /* move d2,d3 */
    if (length == 10) {
        int x1 = input.at(5).toAscii() - 'a';
        int y1 = 7 - input.mid(6, 1).toInt();
        int i1 = getIdx(x1, y1);
        if (i1 == -1) return -1;
        int x2 = input.at(8).toAscii() - 'a';
        int y2 = 7 - input.mid(9, 1).toInt();
        int i2 = getIdx(x2, y2);
        if (i2 == -1) return -1;
        return moveCheck(i1, i2, makeMove);
    }

    /* move a1d1,f4 */
    if (length == 12) {
        int x1 = input.at(5).toAscii() - 'a';
        int y1 = 7 - input.mid(6, 1).toInt();
        int i1 = getIdx(x1, y1);
        if (i1 == -1) return -1;
        int x2 = input.at(7).toAscii() - 'a';
        int y2 = 7 - input.mid(8, 1).toInt();
        int i2 = getIdx(x2, y2);
        if (i2 == -1) return -1;
        int comma = input.at(9).toAscii();
        if (comma != ',') return -1;
        int x3 = input.at(10).toAscii() - 'a';
        int y3 = 7 - input.mid(11, 1).toInt();
        int i3 = getIdx(x3, y3);
        if (i3 == -1) return -1;
        return moveCheck(i1, i2, i3, makeMove);
    }
    return -1;
}

/******************************************************************************
 *
 * Checks if the user has any man in no mill.
 * I needs to check that picking from mill is legal or not.
 * I hasSoloMorris(color) is false it is allowed to pick from mill.
 *
 ******************************************************************************/
bool Mill::hasSoloMorris(int color) {
    for (int i = 0; i < 24; i++)
        if (table->table[i] == color)
            if (! isMill(i, color))
                return true;
    return false;
}

/******************************************************************************
 *
 * Gets the move: "move d2", makes the move and saves it
 * Returns -1 if illegal move
 *
 ******************************************************************************/
int Mill::move(QString input, bool updateHistory) {
    int res = moveCheck(input, true);
    vector<string> moves;
    if (res == 0) {
        if (updateHistory) {
            int size = history.size() - historyIdx - 1;
            for (int i = 0; i < size; i++){
                history.pop_back();
            }
            history.push_back(input);
            historyIdx = history.size() - 1;
        }
        return 0;
    } else {
        cout << "Invalid move: " << input.toStdString() << endl;
        return -1;
    }
}

/******************************************************************************
 *
 * Gets the index of the field from coordinates
 *
 ******************************************************************************/
int Mill::getIdx(int x, int y) {
    for (int i = 0; i < 24; ++i) {
        if (coordHelper[i * 3 + 1] == y &&
                coordHelper[i * 3 + 2] == x)
            return i;
    }
    return -1;
}

/******************************************************************************
 *
 * Returns if it is mill or not
 *
 ******************************************************************************/
bool Mill::isMill(int idx, int color) {
    if (table->table[mill[idx][0]] == color &&
            table->table[mill[idx][1]] == color) return true;
    if (table->table[mill[idx][2]] == color &&
            table->table[mill[idx][3]] == color) return true;
    return false;
}

/******************************************************************************
 *
 * Returns true if idx1 and idx2 are neighbours, otherwise false
 *
 ******************************************************************************/
bool Mill::isNeighbor(int idx1, int idx2) {
    return millNeighbors[idx1 * 5 + 1] == idx2 ||
        millNeighbors[idx1 * 5 + 2] == idx2 ||
        millNeighbors[idx1 * 5 + 3] == idx2 ||
        millNeighbors[idx1 * 5 + 4] == idx2;
}

/******************************************************************************
 *
 * Returns the number of men of a color
 *
 ******************************************************************************/
int Mill::getNofPiece(int color) {
    int n = 0;
    for (int i = 0; i < 24; i++) {
        if (table->table[i] == color) {
            n++;
        }
    }
    return n;
}

/******************************************************************************
 *
 * Returns 0 if not end, 1 if white won, -1 if black won
 *
 ******************************************************************************/
int Mill::isEnd() {
    if (table->whiteHand > 0) return 0;
    if (table->blackHand > 0) return 0;
    if (getNofPiece(WHITE) < 3) return -1;
    if (getNofPiece(BLACK) < 3) return 1;
    int n_moves = getAllMoves().size();
    if (table->whiteToMove && n_moves == 0) return -1;
    if (! table->whiteToMove && n_moves == 0) return 1;
    return 0;
}

/******************************************************************************
 *
 * Returns all the legal moves.
 * It uses brute force to generate all the moves and checks if it is legal
 * one by one.
 *
 ******************************************************************************/
vector<string> Mill::getAllMoves() {
    vector<string> moves;
    char move[4];
    if (table->whiteToMove) {
        if (table->whiteHand > 0) {
            for (int i = 0; i < 24; i++) {
                if (moveCheck(i, false) == 0) {
                    sprintf(move, "%c", i + 'a');
                    moves.push_back(move);
                } else {
                    for (int j = 0; j < 24; j++) {
                        if (moveCheck(i, j, false) == 0) {
                            sprintf(move, "%c%c", i + 'a', j + 'a');
                            moves.push_back(move);
                        }
                    }
                }
            }
        } else {
            for (int i = 0; i < 24; i++) {
                for (int j = 0; j < 24; j++) {
                    if (moveCheck(i, j, false) == 0) {
                        sprintf(move, "%c%c", i + 'a', j + 'a');
                        moves.push_back(move);
                    } else {
                        for (int k = 0; k < 24; k++) {
                            if (moveCheck(i, j, k, false) == 0) {
                                sprintf(move, "%c%c%c", i + 'a', j + 'a', k + 'a');
                                moves.push_back(move);
                            }
                        }
                    }
                }
            }
        }
    } else {
        if (table->blackHand > 0) {
            for (int i = 0; i < 24; i++) {
                if (moveCheck(i, false) == 0) {
                    sprintf(move, "%c", i + 'a');
                    moves.push_back(move);
                } else {
                    for (int j = 0; j < 24; j++) {
                        if (moveCheck(i, j, false) == 0) {
                            sprintf(move, "%c%c", i + 'a', j + 'a');
                            moves.push_back(move);
                        }
                    }
                }
            }
        } else {
            for (int i = 0; i < 24; i++) {
                for (int j = 0; j < 24; j++) {
                    if (moveCheck(i, j, false) == 0) {
                        sprintf(move, "%c%c", i + 'a', j + 'a');
                        moves.push_back(move);
                    } else {
                        for (int k = 0; k < 24; k++) {
                            if (moveCheck(i, j, k, false) == 0) {
                                sprintf(move, "%c%c%c", i + 'a', j + 'a', k + 'a');
                                moves.push_back(move);
                            }
                        }
                    }
                }
            }
        }
    }
    /*
       for (int i = 0; i < 24; i++) {
       if (moveCheck(i, false) == 0) {
       sprintf(move, "%c", i + 'a');
       moves.push_back(move);
       }
       for (int j = 0; j < 24; j++) {
       if (moveCheck(i, j, false) == 0) {
       sprintf(move, "%c%c", i + 'a', j + 'a');
       moves.push_back(move);
       }
       for (int k = 0; k < 24; k++) {
       if (moveCheck(i, j, k, false) == 0) {
       sprintf(move, "%c%c%c", i + 'a', j + 'a', k + 'a');
       moves.push_back(move);
       }
       }
       }
       }
       */
    return moves;
}

int Mill::getHistoryIdx() {
    return historyIdx;
}

void Mill::setHistoryIdx(int idx) {
    historyIdx = idx;
}

/******************************************************************************
 *
 * Updates the table according to the history
 *
 ******************************************************************************/
void Mill::updateTable() {
    initTable(false);
    for (int i = 0; i <= historyIdx; i++) {
        moveCheck(history[i], true);
    }
}

/******************************************************************************
 *
 * Prints the table to the console. Only for testing.
 *
 ******************************************************************************/
void Mill::printTable() {
    int *t = table->table;
    printf("%s\n", table->whiteToMove ? "White to move" : "Black to move");
    printf("7 %d-----%d-----%d Black hand: %d\n", *t, *(t+1), *(t+2), table->blackHand);
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
    printf("1 %d-----%d-----%d White hand: %d\n", *(t+21), *(t+22), *(t+23), table->whiteHand);
    printf("  A B C D E F G\n");
}

/******************************************************************************
 *
 * Converts move from Char to notation.
 * Eg. move: a, returns: move a7
 *
 ******************************************************************************/
string Mill::convertMoveToCoord(string move) {
    int size = move.size();
    char ret[13];
    if (size == 1) {
        int i = move[0] - 'a';
        int x = coordHelper[i*3+2] + 'a';
        int y = 7 - coordHelper[i*3+1];
        sprintf(ret, "move %c%d", x, y);
    } else if (size == 2) {
        int i1 = move[0] - 'a';
        int x1 = coordHelper[i1*3+2] + 'a';
        int y1 = 7 - coordHelper[i1*3+1];
        int i2 = move[1] - 'a';
        int x2 = coordHelper[i2*3+2] + 'a';
        int y2 = 7 - coordHelper[i2*3+1];
        if (table->whiteToMove && table->getWhiteHand() > 0) {
            sprintf(ret, "move %c%d,%c%d", x1, y1, x2, y2);
        } else if (! table->whiteToMove && table->getBlackHand() > 0) {
            sprintf(ret, "move %c%d,%c%d", x1, y1, x2, y2);
        } else {
            sprintf(ret, "move %c%d%c%d", x1, y1, x2, y2);
        }
    } else if (size == 3) {
        int i1 = move[0] - 'a';
        int x1 = coordHelper[i1*3+2] + 'a';
        int y1 = 7 - coordHelper[i1*3+1];
        int i2 = move[1] - 'a';
        int x2 = coordHelper[i2*3+2] + 'a';
        int y2 = 7 - coordHelper[i2*3+1];
        int i3 = move[2] - 'a';
        int x3 = coordHelper[i3*3+2] + 'a';
        int y3 = 7 - coordHelper[i3*3+1];
        sprintf(ret, "move %c%d%c%d,%c%d", x1, y1, x2, y2, x3, y3);
    }
    return ret;
}

/******************************************************************************
 *
 * Sets any position. It is good for testing.
 * t contains the table
 *
 ******************************************************************************/
void Mill::setPos(int *t, int whiteHand, int blackHand, bool whiteToMove) {
    for (int i = 0; i < 24; i++)
        table->table[i] = *(t+i);
    table->whiteHand = whiteHand;
    table->blackHand = blackHand;
    table->whiteToMove = whiteToMove;
}

int Mill::getNofNeighbors(int color) {
    int n = 0;
    for (int i = 0; i < 24; i++) {
        if (table->table[i] == color) {
            n += nofNeighbors[i * 2 + 1];
        }
    }
    return n;
}

void Mill::backupPosition() {
    whiteToMove_backup = table->whiteToMove;
    whiteHand_backup = table->whiteHand;
    blackHand_backup = table->blackHand;
    for (int i = 0; i < 24; i++) t_backup[i] = table->table[i];
}

void Mill::backupPosition(const Mill *mill) {
    whiteToMove_backup = mill->table->whiteToMove;
    whiteHand_backup = mill->table->whiteHand;
    blackHand_backup = mill->table->blackHand;
    for (int i = 0; i < 24; i++) t_backup[i] = mill->table->table[i];
}

void Mill::restorePosition() {
    table->whiteToMove = whiteToMove_backup;
    table->whiteHand = whiteHand_backup;
    table->blackHand = blackHand_backup;
    for (int i = 0; i < 24; i++) table->table[i] = t_backup[i];
}

void Mill::restorePosition(Mill *mill) {
    mill->table->whiteToMove = whiteToMove_backup;
    mill->table->whiteHand = whiteHand_backup;
    mill->table->blackHand = blackHand_backup;
    for (int i = 0; i < 24; i++) mill->table->table[i] = t_backup[i];
}

string Mill::getBestMoveMCTS() {
    Move *move = new Move(this);
    string bestMove = "";
    time_t start, end;
    time(&start);
    for (int i = 0; i < n; i++) {
        //printf("%d.\n", i);
        //printTable();
        backupPosition();
        move->selectAction();
        restorePosition();
    }
    time(&end);
    bestMove = move->getBest()->currMove;
    for (Move *c : move->getChildren()) {
        c->print();
    }
    printf("best move: ");
    move->getBest()->print();
    printf("elapsed time: %ld\n", end - start);
    delete move;
    return convertMoveToCoord(bestMove);
}
