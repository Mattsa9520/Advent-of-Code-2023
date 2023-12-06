#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_GAMES 100
#define LINE_BUF_SIZE 1024 
#define RED_CUBES 12
#define GREEN_CUBES 13
#define BLUE_CUBES 14

/*
 * =======================================
 * ** Advent of Code 2023 - Day 2 **
 *
 * First star solution uses update_games_str(const char*)
 * it takes the provided string that is formated as specified in the game.
 * It reads the id of the game and parses through subsets of cubes and records
 * the amount for each color. If the amount is the maximum, then it is stored 
 * in the game that corresponds to the id. Then
 * is_game_possible(int, int, int, int) checks the game at the given parameter
 * and checks if the given game has a number of cubes that exceeds the maximum
 * for any of the colors. If so, it will return 0. If the game does meet
 * the requirements, then the game's id is returned as it is
 * seen in the file format. These ids are summed up to get the answer to the first
 * part
 *
 * Second star solution generates the power of each game by multiplying the
 * maximum values together. These values are summed up for each game
 * and return the answer
 *
 * This solution generates the correct answers with the given input in "tmp/my_input.txt"
 * ======================================
 * */

typedef struct Game {
    int red_max;
    int green_max;
    int blue_max;
} Game;

static Game games[MAX_GAMES] = { 0 };

void update_games_str(const char* str) {
    if (str == NULL || strlen(str) <= 1) return;

    char* current = strstr(str, " " );
    size_t id = atoi(current+1);

    Game *game = &games[id-1];

    game->red_max = INT_MIN;
    game->blue_max = INT_MIN;
    game->green_max = INT_MIN;

    current = strstr(current+1, " ");
    int amount;

    while (current != NULL) {
        switch (current[1]) {
            case 'r':
                if (amount > game->red_max) {
                    game->red_max = amount;
                }
               break;
            case 'b':
                if (amount > game->blue_max) {
                    game->blue_max = amount;
                }
                break;
            case 'g':
                if (amount > game->green_max) {
                    game->green_max = amount;
                }
               break;
            default:
                {
                    amount = atoi(current+1);
                }
                break;
        }

        current = strstr(current+1, " ");
    }
}

int is_game_possible(int index, int red_con, int green_con, int blue_con) {
    if (red_con >= games[index].red_max && green_con >= games[index].green_max && blue_con >= games[index].blue_max) {
        return index + 1;
    }

    return 0;
}

long long game_min_power(int index) {
    long long power = games[index].red_max * games[index].green_max * games[index].blue_max;
    return power;
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        printf("Enter input filename as argument\n");
        return 1;
    }

    FILE* fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: Failed to open file [%s]\n", argv[1]);
        return 1;
    }

    char line[LINE_BUF_SIZE];
    while (fgets(line, LINE_BUF_SIZE, fp)) {
        update_games_str(line);
    }

    printf("Finished updating!\n");

    int id_sum = 0;
    long long power_sum = 0;
    for (int i = 0; i < MAX_GAMES; i++) {
        printf("Game %d: %d max_red, %d max_green, %d max_blue\n", i+1, games[i].red_max, 
                games[i].green_max, games[i].blue_max);
        id_sum += is_game_possible(i, RED_CUBES, GREEN_CUBES, BLUE_CUBES);

        long long min_power = game_min_power(i);
        printf("%d min power: %lli\n", i, min_power);
        power_sum += min_power;
    }

    printf("Sum of ids that are possible: %d\n", id_sum);
    printf("Sum of min poweres: %lli\n", power_sum);
    return 0;
}
