#include "utils.h"

void **newMatrix(long int x, long int y, long elemSize) {
  void **matrix = (void *)malloc(x*sizeof(void *));
  if(matrix != NULL) {
    int i;
    for(i = 0; i < x; i++) {
      matrix[i] = malloc(y*elemSize);

      if(matrix[i] == NULL) {
        while(--i >= 0) {
          free(matrix[i]);
        }
        free(matrix);
        return NULL;
      }
    }
  }

  return matrix;
}

void deleteMatrix(void **matrix, long int x) {
  int i;
  for(i = 0; i < x; i++) {
    free(matrix[i]);
  }
  free(matrix);
}
