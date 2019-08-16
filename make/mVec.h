#ifndef __MVEC_H__
#define __MVEC_H__

const int MAX_VECTOR_SIZE = 10000;

static int size = 0; 
static int* pVector; 

void createVector();
int getSizeVector();
int getElemAtVector(int pos);
void addElemAtVector(int val);
void setElemAtVector(int pos, int val);
void delElemAtVector(int pos);
int getMaxElemAtVector();
int _max(int a, int b);
void deleteVector();

#endif 
