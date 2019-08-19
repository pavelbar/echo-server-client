#include <stdlib.h>  // malloc()
#include <stdio.h>   // perror(), printf
#include "mVec.h"

void createVector(int** pVector) {
  (*pVector) = (int*)malloc(MAX_VECTOR_SIZE * sizeof(int));
}

int getSizeVector() {
  return size;
}

int getElemAtVector(int pos, int** pVector) {
  if ((pos < 0) || (pos >= size)) {
    perror("Error in getElemVector() \n");
  }
  return (*pVector)[pos];
}

void addElemAtVector(int val, int** pVector) {
  if (size == MAX_VECTOR_SIZE) {
    perror("Error in addElemAtVector() \n");
  }

  (*pVector)[size] = val;
  size++;
}

void setElemAtVector(int pos, int val, int** pVector) {
  if ((pos < 0) || (pos >= size)) {
    perror("Error in setElemAtVector() \n");
  }
  (*pVector)[pos] = val;
}

void delElemAtVector(int pos, int** pVector) {
  if ((pos < 0) || (pos >= size)) {
    perror("Error in delElemAtVector() \n");
  }
  int oldInd = 0;
  int newInd = 0;
  for (oldInd = 0; oldInd < size; oldInd++) {
    if (oldInd != pos) {
      (*pVector)[newInd] = (*pVector)[oldInd];
      newInd++;
    }
  }
  size--;
}

int getMaxElemAtVector(int** pVector) {
  int max = 0;
  if (size != 0) {
    max = (*pVector)[0];
  } else {
    return 0;
    // perror("Error in getMaxElemAtVector() \n");
  }

  int i = 0;
  for (i = 0; i < size; ++i) {
    if ((*pVector)[i] > max) {
      max = (*pVector)[i];
    }
  }
  return max;
}

void deleteVector(int** pVector) {
  free((*pVector));
}
