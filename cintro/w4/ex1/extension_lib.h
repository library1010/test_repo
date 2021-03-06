#ifndef __EXTENSION_LIB_H__
#define __EXTENSION_LIB_H__

#include <time.h>

#include "my_input_lib.h"

#define MAXLENGTH 100
#define MAXSIZE 1000

int solve();
void printIntList(int *a, int amount);
void selection(int *a, int n);
void swap(int *a, int *b);
int findTheElement(int *a, int amount, int rank);

#endif