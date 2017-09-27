#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

void **newMatrix(long int x, long int y, long elemSize);
void deleteMatrix(void **matrix, long int x);

#endif
