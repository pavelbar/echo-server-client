#ifndef __MVEC_H__
#define __MVEC_H__

static const int MAX_VECTOR_SIZE = 10000;

static int size = 0; 

void createVector(int** pVector);
int getSizeVector();
int getElemAtVector(int pos, int** pVector);
void addElemAtVector(int val, int** pVector);
void setElemAtVector(int pos, int val, int** pVector);
void delElemAtVector(int pos, int** pVector);
int getMaxElemAtVector(int** pVector);
void deleteVector(int** pVector);

#endif   
