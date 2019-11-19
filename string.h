/*
* Predmet  :   IFJ / IAL
* Súbor    :   string.h - Pomocna knihovna lexikálního analyzátoru
* Projekt  :   Implementácia prekladača imperatívneho jazyka IFJ19
* Tým č    :   127
* Varianta :   I
* Autoři   : xhalom00, Ivan Halomi
*            xhiner00, Martin Hiner
*            xsevci64, Adam Ševčík
*            xzakji02, Jiří Žák
*/

#ifndef IFJ_STRING_H
#define IFJ_STRING_H

#include <stdlib.h>
#include <string.h>
#include "err_code.h"

#define INIT_ALLOC_SIZE 10

typedef struct {
    char *value; // retezec
    int length; // delka retezce
    int lengthAllocated; // alokovana delka
} string;

int stringInit(string *);

int stringAddChar(string *, char);

int stringAddString(string *, char *);

void stringChangeLastChar(string *, char);

void stringDeleteLastChar(string *);

void stringFree(string *);

#endif //IFJ_STRING_H
