#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * ==========================================
 * ** Advent of Code - Day 3 **
 *
 * First star solution reads the input from the file
 * and appends it into a single string, removing the newlines. 
 * It stores the length of each line. Whenever it encounters a number 
 * it reads the number from the string, and checks all viable spots
 * around that number for symbols. If a symbol is encountered, it 
 * adds it to the sum. If it doesn't it skips ahead past the 
 * found number. 
 *
 * Second star solution, when checking if the number is surrounded by a symbol, 
 * it the symbol is a '*', it accesses the gear associated with that symbol, 
 * and updates the correct gear. Then at the end, the program iterates over the
 * gears, calculates the ratios, and then adds them together
 *
 * This generates the correct answer with the given input in "tmp/my_input.txt"
 * ==========================================
 * */

#define SYMBOL_SIZE 14
#define MAX_GEARS 256*256 

const char symbols[SYMBOL_SIZE] = {
    '!',
    '@',
    '#',
    '$',
    '$',
    '%',
    '^',
    '&',
    '*',
    '(',
    '/',
    '-',
    '=',
    '+'
};

typedef struct {
    int gear1;
    int gear2;
} Gear;

Gear gears[MAX_GEARS];

void gears_init() {
    for (int i = 0; i < MAX_GEARS; i++) {
        gears[i].gear1 = -1;
        gears[i].gear2 = -1;
    }
}

int num_length(const int num) {
    int temp = num;

    int count = 0;
    while (temp > 0) {
        temp /= 10;
        count++;
    }

    return count; 
}

bool is_symbol(const char c) {
    for (int i = 0; i < SYMBOL_SIZE; i++) {
        if (c == symbols[i]) {
            return true;
        }
    }

    return false;
}

int map(int x, int in_bot, int in_top, int out_bot, int out_top) {
    return (x - in_bot) * ((out_top-out_bot)/(in_top-in_bot)) + out_bot;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char schematic[256*256];
    gears_init();
    printf("gear[0]: 1 %d 2 %d\n", gears[0].gear1, gears[1].gear2);

    FILE* fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: Failed to open file [%s]\n", argv[1]);
        return 1;
    }

    char line[256];
    int line_len = 0;
    if (fgets(line, 256, fp)) {
        line[strcspn(line, "\n")] = 0;
        line_len = strlen(line);
        strncat(schematic, line, line_len);
    }

    while (fgets(line, 256, fp)) {
        line[strcspn(line, "\n")] = 0;
        strncat(schematic, line, line_len);
    }

    char* s = schematic;
    printf("line_len = %d\n", line_len);
    int n = strlen(schematic);
    printf("schem_len = %d\n", n);
    int sum = 0;
    
    int starIndex = -1;
    int gear1 = -1;
    int gear2 = -1;
    int gearSum = 0;

    for (int i = 0; i < n; i++) {
        if (!isdigit(schematic[i])) {
            continue;
        }

        s = schematic + i;

        int num = strtol(s, &s, 10);
        int len = num_length(num);
        
        printf("num = %d; len = %d\n", num, len);
        bool add = false;

        // above
        if (i >= line_len) {
            for (int j = (i-(line_len+1)); j <= ((i-(line_len+1))+(len+1)); j++) {
                if (is_symbol(schematic[j])) {
                    sum += num;
                    add = true;

                    if (schematic[j] == '*') {
                        Gear *gear = &gears[map(j, 0, n, 0, MAX_GEARS)];
                        if (gear->gear1 == -1) {
                            gear->gear1 = num;
                        } else if (gear->gear2 == -1) {
                            gear->gear2 = num;
                        }
                    }
                }
            }
        }

        // left
        if (i > 0) {
            if (is_symbol(schematic[i-1])) {
                sum += num;
                add = true;

                if (schematic[i-1] == '*') {
                    Gear *gear = &gears[map(i-1, 0, n, 0, MAX_GEARS)];
                    if (gear->gear1 == -1) {
                        gear->gear1 = num;
                    } else if (gear->gear2 == -1) {
                        gear->gear2 = num;
                    }
                    
                }
            }
        }

        // right
        if ((i + len) < (n-1)) {
            if (is_symbol(schematic[(i+len)])) {
                sum += num;
                add = true;

                if (schematic[i+len] == '*') {
                    Gear *gear = &gears[map(i+len, 0, n, 0, MAX_GEARS)];
                    if (gear->gear1 == -1) {
                        gear->gear1 = num;
                    } else if (gear->gear2 == -1) {
                        gear->gear2 = num;
                    }
                }
            }
        }

        // below
        if (i <= ((n-1)-line_len)) {
            for (int j = (i+(line_len-1)); j <= ((i+(line_len-1))+(len+1)); j++) {
                if (is_symbol(schematic[j])) {
                    sum += num;
                    add = true;
                    
                    if (schematic[j] == '*') {
                        Gear *gear = &gears[map(j, 0, n, 0, MAX_GEARS)];
                        if (gear->gear1 == -1) {
                            gear->gear1 = num;
                        } else if (gear->gear2 == -1) {
                            gear->gear2 = num;
                        }
                    }
                }
            }
        }

        if (add) printf("%d added\n", num);

        i += (len-1);
    }

    for (int i = 0; i < MAX_GEARS; i++) {
        Gear *gear = &gears[i];
        int ratio = (gear->gear1 == -1 ? 0 : gear->gear1) * (gear->gear2 == -1 ? 0 : gear->gear2);
        gearSum += ratio;
    }

    printf("sum = %d\n", sum);
    printf("gearSum = %d\n", gearSum);
}
