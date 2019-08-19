#ifndef __MVEC_H__
#define __MVEC_H__


struct TNode
{
   struct TNode *pNext;
   struct TNode *pPrev;
   int pos;
   int elem;
} TNode;


static int count; //size

void CreateList(struct TNode **pFirst, struct TNode **pCurr);
static void Next(struct TNode **pCurr);
static void Back(struct TNode **pCurr);
static void FullRePos(struct TNode **pFirst, struct TNode **pCurr);
static void SetPos(int position, struct TNode **pFirst, struct TNode **pCurr);
void InSertFirst(int val, struct TNode **pFirst, struct TNode **pCurr);
void InSertLast(int val, struct TNode **pFirst, struct TNode **pCurr);
int GetElem( int pos, struct TNode **pFirst, struct TNode **pCurr);
void Delete(const int pos, struct TNode **pFirst, struct TNode **pCurr);
void DeleteFirst(struct TNode **pFirst, struct TNode **pCurr);
void DeleteLast(struct TNode **pFirst, struct TNode **pCurr);
void DeleteAll(struct TNode **pFirst, struct TNode **pCurr);
int getMaxElem(struct TNode **pFirst, struct TNode **pCurr);
int GetSize(); 

//============



#endif


