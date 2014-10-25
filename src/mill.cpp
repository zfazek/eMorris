#include "mill.h"
#include "node.h"
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

Mill::Mill() {
    table = new Table();
    this->initRules();
    this->initTable(true);
    n = 500;
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
    table->initTable();
    if (historyClear) {
        history.clear();
        historyIdx = -1;
    }
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

int Mill::moveCheck(Move move, bool updateHistory) {
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
int Mill::moveCheck(int i, bool makeMove) {
    if (table->whiteToMove and table->whiteHand == 0) return -1;
    if (! table->whiteToMove and table->blackHand == 0) return -1;
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
int Mill::moveCheck(int i1, int i2, bool makeMove, int n_white, int n_black) {

    /* Simple WHITE move */
    if (table->whiteToMove && table->whiteHand == 0) {
        if (table->table[i1] != WHITE) return -1;
        if (table->table[i2] != EMPTY) return -1;
        if (n_white > 3 && ! isNeighbor(i1, i2)) return -1;
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
        if (n_black > 3 && ! isNeighbor(i1, i2)) return -1;
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
        if (table->table[i2] != BLACK) return -1;
        if (isMill(i1, WHITE)) {

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
        if (table->table[i2] != WHITE) return -1;
        if (isMill(i1, BLACK)) {

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
int Mill::moveCheck(int i1, int i2, int i3, bool makeMove, int n_white, int n_black) {
    if (table->whiteToMove && table->whiteHand == 0) {
        if (table->table[i1] != WHITE) return -1;
        if (table->table[i2] != EMPTY) return -1;
        if (table->table[i3] != BLACK) return -1;
        if (n_white > 3 && ! isNeighbor(i1, i2)) return -1;
        table->table[i1] = EMPTY;
        table->table[i2] = WHITE;
        if (!isMill(i2, WHITE)) {
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

    if (! table->whiteToMove && table->blackHand == 0) {
        if (table->table[i1] != BLACK) return -1;
        if (table->table[i2] != EMPTY) return -1;
        if (table->table[i3] != WHITE) return -1;
        if (n_black > 3 && ! isNeighbor(i1, i2)) return -1;
        table->table[i1] = EMPTY;
        table->table[i2] = BLACK;
        if (!isMill(i2, BLACK)) {
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

Move Mill::getMove(QString input) {
    Move move;
    move.length = 0;
    int length = input.length();

    /* move d2 */
    if (length == 7) {
        int x = input.at(5).toAscii() - 'a';
        int y = 7 - input.mid(6, 1).toInt();
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
        int y1 = 7 - input.mid(6, 1).toInt();
        int i1 = getIdx(x1, y1);
        if (i1 == -1) return move;
        int x2 = input.at(7).toAscii() - 'a';
        int y2 = 7 - input.mid(8, 1).toInt();
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
        int y1 = 7 - input.mid(6, 1).toInt();
        int i1 = getIdx(x1, y1);
        if (i1 == -1) return move;
        int x2 = input.at(8).toAscii() - 'a';
        int y2 = 7 - input.mid(9, 1).toInt();
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
        int y1 = 7 - input.mid(6, 1).toInt();
        int i1 = getIdx(x1, y1);
        if (i1 == -1) return move;
        int x2 = input.at(7).toAscii() - 'a';
        int y2 = 7 - input.mid(8, 1).toInt();
        int i2 = getIdx(x2, y2);
        if (i2 == -1) return move;
        int comma = input.at(9).toAscii();
        if (comma != ',') return move;
        int x3 = input.at(10).toAscii() - 'a';
        int y3 = 7 - input.mid(11, 1).toInt();
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

/******************************************************************************
 *
 * Checks if the user has any man in no mill.
 * It needs to check that picking from mill is legal or not.
 * If hasSoloMorris(color) is false it is allowed to pick from mill.
 *
 ******************************************************************************/
bool Mill::hasSoloMorris(int color) {
    for (int i = 0; i < 24; i++)
        if (table->table[i] == color)
            if (! isMill(i, color))
                return true;
    return false;
}

int Mill::move(Move move, bool updateHistory) {
    int res = moveCheck(move, true);
    if (res == 0) {
        if (updateHistory) {
            int size = history.size() - historyIdx - 1;
            for (int i = 0; i < size; i++){
                history.pop_back();
            }
            history.push_back(QString::fromStdString(move.toString()));
            historyIdx = history.size() - 1;
        }
        return 0;
    } else {
        cout << "Invalid move: " << move.toString() << endl;
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
vector<Move> Mill::getAllMoves() {
    vector<Move> moves;
    int n_white = getNofPiece(WHITE);
    int n_black = getNofPiece(BLACK);
    if (table->whiteToMove) {
        if (table->whiteHand > 0) {
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
        if (table->blackHand > 0) {
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
        moveCheck(getMove(history[i]), true);
    }
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
    Node *move = new Node(this);
    string bestMove = "";
    time_t start, end;
    time(&start);
    for (int i = 0; i < n; i++) {
        backupPosition();
        move->selectAction();
        restorePosition();
    }
    time(&end);
    bestMove = move->getBest()->currMove.toString();
    for (Node *c : move->getChildren()) {
        c->print();
    }
    printf("best move: ");
    move->getBest()->print();
    printf("elapsed time: %ld\n", end - start);
    delete move;
    return bestMove;
}
