#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>

/**
 * Splices the given string based on parameters.
 * Param:
 * splicedString - the string to place the splice into.
 * string - the string to splice.
 * start - the starting index of the splice.
 * end - the ending index of the splice. The splice only goes up to the
 *          ending index.
 */
void str_splice(char* splicedString, char* string, int start, int end);

/**
 * Counts how many ints in the array are greater than value.
 * Param:
 * array - the int array to check
 * arrayLength - the length of the array
 * value - the number to compare against
 * Returns the number of ints in the array that are greater than value.
 */
int count_greater_than(int* array, int arrayLength, int value);

/**
 * Gets max value in array.
 * Param:
 * array - the int array to check
 * arrayLength - the length of the array
 * Returns the max value in the array.
 */
int max(int* array, int arrayLength);


/**
 * Gets min value in array.
 * Param:
 * array - the int array to check
 * arrayLength - the length of the array
 * Returns the min value in the array.
 */
int min(int* array, int arrayLength);

/**
 * Reads a file from file
 * Param:
 * file - Pointer to file to be read.
 * Returns the line that was read as a string.
 */
char* read_line(FILE* file);

#endif