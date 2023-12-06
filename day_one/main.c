#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "binary_search_tree.h"

/* ==========================================
 *  ** Advent of Code 2023 - Day 1 **
 *
 *  First star solution uses combiend_from_str(const char*)
 *  This simply loops through the string to find any integers, 
 *  and stores a pointer to an array. The pointer points 
 *  to the beginning of the array, where the first encountered
 *  integer is placed. Then the pointer moves to the array's second
 *  index. It continues to update the integer at this index until the end
 *  of the string is reached. If no second integer is found, the second index
 *  is the same as the first 
 *
 *  Second star solution using a Binary Search Tree to store the minimum 
 *  and maximum index of found digits. Digits in this scenary can be any
 *  integer 0-9 within a string or the words stored in DIGIT_STRINGS
 *  The algorithm is meant to work the same as the previous star
 *  excpet the digit strings count as well. The function loops over
 *  the string, if it encouters a digit, it adds the index of that digit
 *  to the BST. This makes it convenient to find the largest and smallest
 *  indexes later. The indexes are pointer offsets in the string pointing to 
 *  the location of either an actual digit, or the beginning of a digit string
 *  Only indexes pointing to a substring with the smallest number of characters that 
 *  still holds the digit strings are allowed, because technically, every index 
 *  upto and including the digit string, would count.
 *
 *  This solution generates the correct answers with the given input in "tmp/my_input.txt"
 *
 *
 * =========================================
 * */

#define MAX_BUFFER_SIZE 256
#define ASCII_OFFSET 48

const char* DIGIT_STRINGS[9] = {
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine"
};

int combined_from_str(const char* str) {
    int res = 10;

    int found[2];
    int* curr = found;

    for (int i = 0; i < strlen(str); i++) {
        if (isdigit(str[i])) {
            *curr = (int)(str[i] - ASCII_OFFSET);
            if (curr < (found + 1)) {
                *(curr + 1) = *curr;
                curr++;
            }
        }
    }

    res = (res * found[0]) + found[1];

    return res;
}

bool is_digit_str(const char* str) {

    for (int i = 0; i < 9; i++) {
        const char* digitStr = strstr(str, DIGIT_STRINGS[i]);
        if (digitStr != NULL && digitStr == str) {
            return true;
        }
    }

    return false;
}

int digit_str_to_int(const char* str) {
    if (str == NULL) return 0;

    const char* digit_str = str + strlen(str);
    int number = 0;
    for (int i = 0; i < 9; i++) {
        if (strstr(str, DIGIT_STRINGS[i]) != NULL) {
            if (strstr(str, DIGIT_STRINGS[i]) <= digit_str) {
                digit_str = strstr(str, DIGIT_STRINGS[i]);
                number = i + 1;
            }
        }
    }

    return number;
}

int combined_from_str_pro(const char* str) {
    int res = 10;

    BinarySearchTree* tree = BST_Create();

    for (int i = 0; i < strlen(str); i++) {
        if (isdigit(str[i])) {
            BST_Insert(tree, i);
        } else if (is_digit_str(str + i)) {
            BST_Insert(tree, i);
        }
    }

    int min;

    if (BST_Min(tree) != NULL) {
        min = *BST_Min(tree);
        if (isdigit(str[min])) {
            min = (int)(str[min] - ASCII_OFFSET);
        } else if (is_digit_str(str + min)) {
            min = digit_str_to_int(str + min);
        }
    }

    int max;
    if (BST_Max(tree) != NULL) {
        max = *BST_Max(tree);
        if (isdigit(str[max])) {
            max = (int)(str[max] - ASCII_OFFSET);
        } else {
            max = digit_str_to_int(str + max);
        }
    }

    BST_Destory(tree);

    res = (res * min) + max;
    
    return res;
}

int sum_from_file(const char* fileName) {
    FILE* fp = fopen(fileName, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: Failed to open file [%s]\n", fileName);
        return -1;
    }

    char buffer[MAX_BUFFER_SIZE];

    int sum = 0;

    while (fgets(buffer, MAX_BUFFER_SIZE, fp)) {
        sum += combined_from_str(buffer);
    }

    fclose(fp);

    return sum;
}

int sum_from_file_pro(const char* fileName) {
    FILE* fp = fopen(fileName, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: Failed to open file [%s]\n", fileName);
        return -1;
    }

    char buffer[MAX_BUFFER_SIZE];

    int sum = 0;

    while (fgets(buffer, MAX_BUFFER_SIZE, fp)) {
        int combined = combined_from_str_pro(buffer);
        printf("string: [%s] int from string: [%d]\n", buffer, combined);
        sum += combined;
    }
    
    fclose(fp);

    return sum;
}

int main(int argc, char* argv[]) {
    
    if (argc <= 1) {
        printf("Enter filename as argument\n");
        return 1;
    }

    int sum = sum_from_file_pro(argv[1]);

    printf("sum = %d\n", sum);

    return 0;
}
