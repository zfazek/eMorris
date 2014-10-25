#include "table.h"
#include <stdio.h>

Table::Table() {
}

int Table::getWhiteHand() {
    return whiteHand;
}

int Table::getBlackHand() {
    return blackHand;
}

void Table::initTable() {
    whiteHand = 9;
    blackHand = 9;
    for (int i = 0; i < 24; ++i) {
        table[i] = EMPTY;
    }
    whiteToMove = true;
}

/******************************************************************************
 *
 * Prints the table to the console. Only for testing.
 *
 ******************************************************************************/
void Table::printTable() {
    int *t = table;
    printf("%s\n", whiteToMove ? "White to move" : "Black to move");
    printf("7 %d-----%d-----%d Black hand: %d\n", *t, *(t+1), *(t+2), blackHand);
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
    printf("1 %d-----%d-----%d White hand: %d\n", *(t+21), *(t+22), *(t+23), whiteHand);
    printf("  A B C D E F G\n");
}

