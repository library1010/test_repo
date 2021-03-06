#ifndef __EXTENSION_LIB_H__
#define __EXTENSION_LIB_H__

#include <time.h>

#include "my_input_lib.h"
#include "file_handle.h"
#include "graph.h"

#define MAXSIZE 100
#define SIZE 100
#define TEMPSIZE 50

int solve(FILE *f);
void createData(Graph g);
int getPerson();

int intCompare(Jval a, Jval b);

#endif