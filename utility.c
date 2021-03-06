#include "utility.h"
#include <stdlib.h>

/**
 * Splices the given string based on parameters.
 * Param:
 * splicedString - the string to place the splice into.
 * string - the string to splice.
 * start - the starting index of the splice.
 * end - the ending index of the splice. The splice only goes up to the
 *          ending index.
 */
void str_splice(char* splicedString, char* string, int start, int end) {
    int length = end - start;
    for (int i = 0; i < length; i++) {
        splicedString[i] = string[start + i];
    }
    splicedString[length] = '\0';
}

/**
 * Counts how many ints in the array are greater than value.
 * Param:
 * array - the int array to check
 * arrayLength - the length of the array
 * value - the number to compare against
 * Returns the number of ints in the array that are greater than value.
 */
int count_greater_than(int* array, int arrayLength, int value) {
    int counter = 0;
    for (int i = 0; i < arrayLength; i++) {
        if (array[i] > value) {
            counter++;
        }
    }
    return counter;
}

/**
 * Gets max value in array.
 * Param:
 * array - the int array to check
 * arrayLength - the length of the array
 * Returns the max value in the array.
 */
int max(int* array, int arrayLength) {
    int max = array[0];
    for (int i = 0; i < arrayLength; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }
    return max;
}

/**
 * Gets min value in array.
 * Param:
 * array - the int array to check
 * arrayLength - the length of the array
 * Returns the min value in the array.
 */
int min(int* array, int arrayLength) {
    int min = array[0];
    for (int i = 0; i < arrayLength; i++) {
        if (array[i] < min) {
            min = array[i];
        }
    }
    return min;
}

/**
 * Reads a file from file
 * Param:
 * file - Pointer to file to be read.
 * Returns the line that was read as a string.
 */
char* read_line(FILE* file) {
    int bufferSize = 80;
    char* result = (char*) malloc(sizeof(char) * bufferSize);
    int position = 0;
    int next = 0;

    while (1) {
        if (position > bufferSize - 2) {
            bufferSize *= 1.5;
            char* buffer = realloc(result, bufferSize);
            if (buffer == NULL) {
                free(result);
                return NULL;
            }
        }
        next = fgetc(file);
        if (next == EOF || next == '\n') {
            result[position] = '\0';
            return result;
        } else {
            result[position++] = (char) next;
        }
    }
}