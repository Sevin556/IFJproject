/*
* Predmet  :   IFJ / IAL
* Súbor    :   stack.c - Zasobnik
* Projekt  :   Implementácia prekladača imperatívneho jazyka IFJ19
* Tým č    :   127
* Varianta :   I
* Autoři   : xhalom00, Ivan Halomi
*            xhiner00, Martin Hiner
*            xsevci64, Adam Ševčík
*            xzakji02, Jiří Žák
*/

#include "stack.h"

void SInit(ptrStack *ptr) {
    ptr->first = NULL;
    ptr->numberOfItems = 0;
}

bool SEmpty(ptrStack *ptr) {
    if (ptr->numberOfItems == 0 && ptr->first == NULL)
        return true;
    return false;
}

bool SPush(ptrStack *ptr, void *value) {
    stackItem *tmp = malloc(sizeof(struct tStackItem));
    if (tmp != NULL) {
        tmp->next = ptr->first;
        ptr->first = tmp;
        tmp->value = value;
        ptr->numberOfItems++;
        return true;
    }
    return false;
}

bool SPop(ptrStack *ptr) {
    stackItem *tmp = ptr->first;
    while (ptr->first != NULL) {
        tmp = ptr->first;
        ptr->first = ptr->first->next;
        free(tmp);
        ptr->numberOfItems--;
        return true;
    }
    return false;
}

bool SDispose(ptrStack *ptr) {
    while (ptr->numberOfItems != 0 && ptr->first != NULL) {
        SPop(ptr);
    }
    return true;
}