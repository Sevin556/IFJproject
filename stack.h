/*
* Predmet  :   IFJ / IAL
* Súbor    :   stack.h - Zasobnik
* Projekt  :   Implementácia prekladača imperatívneho jazyka IFJ19
* Tým č    :   127
* Varianta :   I
* Autoři   : xhalom00, Ivan Halomi
*            xhiner00, Martin Hiner
*            xsevci64, Adam Ševčík
*            xzakji02, Jiří Žák
*/

#ifndef IFJ_STACK_H
#define IFJ_STACK_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct tStackItem {
    struct tStackItem *next;
    void *value;
} stackItem;

typedef struct {
    int numberOfItems;
    struct tStackItem *first;
} ptrStack;

void SInit(ptrStack *);

bool SPush(ptrStack *, void *);

bool SPop(ptrStack *);

bool SEmpty(ptrStack *);

bool SDispose(ptrStack *);

#endif //IFJ_STACK_H
