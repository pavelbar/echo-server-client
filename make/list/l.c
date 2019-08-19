#include <stdlib.h>  // malloc()
#include <stdio.h>   // perror(), printf
#include "mVec.h"

void CreateList(struct TNode **pFirst, struct TNode **pCurr){
    (*pFirst) = (*pCurr) = NULL;
    count = 0;
}
void Next(struct TNode **pCurr) 
 {
    if ((*pCurr)->pNext == NULL){ 
    	 (*pCurr) = (*pCurr)->pNext;
    }
 }


void Back(struct TNode **pCurr) 
{
    if ((*pCurr)->pPrev == NULL){ 
    	 (*pCurr) = (*pCurr)->pPrev;
    }
}

 void FullRePos(struct TNode **pFirst, struct TNode **pCurr)
 {
     int i = 0;
     (*pCurr) = (*pFirst);
     while (i != count)
     {
         (*pCurr)->pos = i++;
         Next(pCurr);
     }
 }

 void SetPos(int position, struct TNode **pFirst, struct TNode **pCurr)
 {
     if ((position < 0) || position > count){
     	perror("Invalid position");
     }
         if (position == 0)
         {
             (*pCurr) = (*pFirst);
         }
         else
         {
             while ((*pCurr)->pos < position){
	     printf("(*pCurr)->pos: %d \n", (*pCurr)->pos);
	     printf("position: %d \n", position);
                 Next(pCurr);
	     }
             while ((*pCurr)->pos > position){
                 Back(pCurr);
	     }
         }
 }

void InSertFirst(int val, struct TNode **pFirst, struct TNode **pCurr)
{
    if ((*pFirst) == NULL)
    {
	(*pFirst) = (struct TNode*)malloc(sizeof(struct TNode));
        (*pFirst)->pos = 0;
        (*pFirst)->elem = val;
        (*pFirst)->pNext = (*pFirst)->pPrev = NULL;
        count++;
    }
    else
    {
	struct TNode *p = (struct TNode*)malloc(sizeof(struct TNode));
        p->elem = val;
        p->pNext = (*pFirst);
        p->pPrev = NULL;
        (*pFirst)=(*pCurr) = p;
        struct TNode *next = (*pFirst)->pNext;
        next->pPrev = (*pFirst);
        count++;
        FullRePos(pFirst, pCurr);
    }
}


void InSertLast(int val, struct TNode **pFirst, struct TNode **pCurr)
{
    if ((*pFirst) == NULL)
    {
        InSertFirst(val, pFirst, pCurr);
    }
    else
    {
        SetPos(count - 1, pFirst, pCurr);
        struct TNode *p = (struct TNode*)malloc(sizeof(struct TNode));
        p->elem = val;
        p->pos = count;
        p->pNext = NULL;
        count++;
        (*pCurr)->pNext = p;
        p->pPrev = (*pCurr);
    }
}

int GetElem(int pos, struct TNode **pFirst, struct TNode **pCurr) 
 {
     if (pos<0 || pos>count) {
     	perror("there is no such position");
	}
     SetPos(pos, pFirst, pCurr);
     return (*pCurr)->elem;
 }

 void Delete(int pos, struct TNode **pFirst, struct TNode **pCurr)
 {
     if ((pos < 0) || (pos > count)){
     	perror("Invalid position");
     }
     if ((*pFirst) == NULL) {
     	return;
     }
     
     if (pos == 0){ 
     	DeleteFirst(pFirst, pCurr);
     }
     else if (pos == count - 1){ 
     DeleteLast(pFirst, pCurr);
     }
     else
     {
         SetPos(pos, pFirst, pCurr);
         struct TNode *prev = (*pCurr)->pPrev;
         struct TNode *next = (*pCurr)->pNext;
	 free((*pCurr));
         prev->pNext = next;
         next->pPrev = prev;
         count--;
         FullRePos(pFirst,pCurr);
     }
 }
 
void DeleteFirst(struct TNode **pFirst, struct TNode **pCurr)
{
     if ((*pFirst) == NULL){
      	return;
      }
     struct TNode *p = (*pFirst)->pNext;
     free((*pFirst));
     p->pPrev = NULL;
     (*pCurr) = (*pFirst) = p;
     count--;
     FullRePos(pFirst, pCurr);
}


void DeleteLast(struct TNode **pFirst, struct TNode **pCurr)
{
     if ((*pFirst) == NULL) {
    	 return;
	}
     SetPos(count - 1, pFirst, pCurr);
     struct TNode *p = (*pCurr)->pPrev;
     p->pNext = NULL;
     free( (*pCurr));
     (*pCurr) = p;
     count--;
}

 void DeleteAll(struct TNode **pFirst, struct TNode **pCurr)
 {
   if ((*pFirst) == NULL)
     {
         free((*pFirst));
         free((*pCurr));
     }
     else if (count == 1)
     {
         free((*pFirst));
     }
     else
     {
         SetPos(1, pFirst, pCurr);
         free((*pFirst));
         struct TNode *p;
         while ((*pCurr)->pos != count - 1)
         {
             p = (*pCurr);
             Next(pCurr);
             free(p);
         }
         free((*pCurr));
     }
     
      (*pFirst) = (*pCurr) = NULL;
      count = 0;
 }

int getMaxElem(struct TNode **pFirst, struct TNode **pCurr){
    int max = 0;
    if (count != 0) {
        max = GetElem(0, pFirst, pCurr);
    }
    else {            
        return 0;
        //perror("Error in getMaxElem \n");
    }

    int i = 0;
    for (i = 0; i < count; ++i) {
        if (GetElem(i, pFirst, pCurr) > max) {
            max = GetElem(i, pFirst, pCurr);
        }
    }
    return max;
}

int GetSize(){
	return count;
}
