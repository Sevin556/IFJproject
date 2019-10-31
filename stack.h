//
// Created by zak on 31.10.19.
//

#ifndef IFJ_STACK_H
#define IFJ_STACK_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct tStackItem{
    struct tStackItem *next;
    void *value;
} stackItem;

typedef struct {
    int numberOfItems;
    struct tStackItem *first;
} ptrStack;

void SInit(ptrStack *ptr);
bool SPush(ptrStack *, void *);
bool SPop(ptrStack *);
bool SEmpty(ptrStack *);
bool SDispose(ptrStack *);

#endif //IFJ_STACK_H
