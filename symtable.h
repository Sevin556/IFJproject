/*
* Predmet  :   IFJ / IAL
* Súbor    :   symtable.h - Hlavičkový súbor pre tabulku symbolov
* Projekt  :   Implementácia prekladača imperatívneho jazyka IFJ19
* Tým č    :   127
* Varianta :   I
* Autoři   : xhalom00, Ivan Halomi
*            xhiner00, Martin Hiner
*            xsevci64, Adam Ševčík
*            xzakji02, Jiří Žák
*/

#ifndef _SYMTABLE_H_
#define _SYMTABLE_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct tBTnode {          /* uzol binárneho stromu */
        char* data;               /* uložené dáta */
        struct tnode* lptr;       /* ukazateľ na ľavého potomka */
        struct tnode* rptr;       /* ukazateľ na pravého potomka */
} *tBTNodePtr;

void BTInit(tBTNodePtr*);
tBTNodePtr* BTInsert(char*);
tBTNodePtr* BTSearch(char*);
void BTCopy(tBTNodePtr*, tBTNodePtr*);
void BTDelete(tBTNodePtr*);

#endif    //_SYMTABLE_H_
