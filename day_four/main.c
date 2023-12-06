#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * =================================================
 *  ** Advent of Code - Day 4 **
 *
 *  First star solution. Iterate over each line, and 
 *  calculate the number of wins each card has. The relation 
 *  from number of wins "n" to "p" points is "p = 2^(n-1)";
 *  The points of each line are added together to get the total 
 *  which is the requested answer
 *
 *  Second star solution stores each card in an array. Since each card
 *  is assocated with its line number, this allows for instant lookup.
 *  We keep track of the number of instances that a card has. The card 
 *  updates the instances of "n" cards ahead of it, where "n" is the 
 *  number of wins the current card has. The card adds its current number
 *  of instances to the following cards. This is allowed, as there is 
 *  no card that will make the sequence go backwards, causing a previous 
 *  instance # to be updated. These instances are then added together 
 *  to produce the final instance total
 *
 *  This solution generates the correct answers with the given input in "tmp/my_input.txt"
 * =================================================
 * */

#define BUFFER_SIZE 256

typedef struct Card {
    int instances;
    int winCount;
} Card;

static Card cards[BUFFER_SIZE];

void init_cards() {
    memset(cards, 0, BUFFER_SIZE);

    for (int i = 0; i < BUFFER_SIZE; i++) {
        cards[i].instances = 1;
        cards[i].winCount = 0;
    }
}

int parse_int_arr(char* str, int* arr) {
    if (str == NULL || arr == NULL) return 0;

    int arrLen = 0;

    char* at = str;
    while (strlen(at) > 1) {
        int num = strtol(at, &at, 10);
        arr[arrLen++] = num;
    }

    return arrLen;
}

bool int_arr_contains(const int arr[], int arrLen, int value) {
    if (arr == NULL) return false;

    for (int i = 0; i < arrLen; i++) {
        if (arr[i] == value) {
            return true;
        }
    }

    return false;
}

int calc_num_wins(char* line) {
    if (line == NULL) return 0;

    char* token = strtok(line, ":");
    token = strtok(NULL, ":");

    token = strtok(token, "|");

    int won[32];
    int wLen = parse_int_arr(token, won);

    token = strtok(NULL, "|");
    
    int have[32];
    int hLen = parse_int_arr(token, have);

    int numWins = 0;

    for (int i = 0; i < hLen; i++) {
        if (int_arr_contains(won, wLen, have[i])) {
            numWins++;
        }
    }

    return numWins;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE* fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: Failed to open file [%s]\n", argv[1]);
        return EXIT_FAILURE;
    }

    init_cards();

    int numWins = 0;
    char line[BUFFER_SIZE];
    int lineNo = 1;
    int points = 0;

    while (fgets(line, BUFFER_SIZE, fp)) {
        int lineWin = calc_num_wins(line);
        printf("%d: wins = %d\n", lineNo, lineWin);
        numWins += lineWin;

        cards[lineNo-1].winCount = lineWin;

        int linePoints = lineWin > 0 ? 1 : 0;
        for (int i = 1; i <= lineWin-1; i++) {
            linePoints *= 2;
        }

        points += linePoints;

        for (int i = lineNo; i <= (lineNo + (lineWin-1)); i++) {
            printf("Card %d adding %d instances to Card %d\n", lineNo, cards[lineNo-1].instances, i+1);
            cards[i].instances += cards[lineNo-1].instances;
        }
        lineNo++;
    }

    fclose(fp);

    printf("numWins = %d\n", numWins);

    printf("points = %d\n", points);

    int instanceTotal = 0;
    for (int i = 0; i < (lineNo-1); i++) {
        printf("Card %d: instances %d winCount %d\n", (i+1), cards[i].instances, cards[i].winCount);
        instanceTotal += cards[i].instances;
    }

    printf("instanceTotal = %d\n", instanceTotal);

    return EXIT_SUCCESS;
}
