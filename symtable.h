/*
* Predmet  :   IFJ / IAL
* Súbor    :   symtable.h
* Projekt  :   Implementácia prekladača imperatívneho jazyka IFJ19
* Tým č    :   127
* Varianta :   I
* Autoři   : xhalom00, Ivan Halomi
*            xhiner00, Martin Hiner
*            xsevci64, Adam Ševčík
*            xzakji02, Jiří Žák
*/
#ifndef IFJ_SYMTABLE_H
#define IFJ_SYMTABLE_H

#include <stdbool.h>
#include "string.h"
#include "scanner.h"

typedef enum {
    ndtVariable,     /* nodeDataTypeVariable */
    ndtFunction,     /* nodeDataTypeFunction */
} tNodeDataType;

typedef struct tBSTNode {
    char *Key;                                         /* klic */
    tNodeDataType nodeDataType;                      /* typ dat, ktera uzel uchovava (funkce, promenna) */
    void *Data;                                      /* uzitecny obsah uzlu */
    struct tBSTNode *LPtr;                          /* ukazatel na levy podstrom */
    struct tBSTNode *RPtr;                          /* ukazatel na pravy podstrom */
} *tBSTNodePtr;

typedef struct variable {
    int retType;
} tVariable;

typedef struct function {
    int retType;
    bool defined;
    bool declared;
    int paramCounter;
    string paramName[10];
} tFunction;

typedef struct symtable {
    tBSTNodePtr root;
} tSymtable;

void BSTInit(tBSTNodePtr *);

tBSTNodePtr BSTSearch(tBSTNodePtr, char *);

void BSTInsert(tBSTNodePtr *, char *, void *, tNodeDataType);

void ReplaceByRightmost(tBSTNodePtr, tBSTNodePtr *);

void BSTDelete(tBSTNodePtr *, char *);

void BSTDispose(tBSTNodePtr *);

void symTableInit(tSymtable *);

void symTableInsertVariable(tSymtable *, string);

void symTableInsertFunction(tSymtable *, string);

tBSTNodePtr symTableSearch(tSymtable *, string);

void symTableDelete(tSymtable *, string);

void symTableDispose(tSymtable *);

void symTableInsertVesFunction(tSymtable *);

#endif //IFJ_SYMTABLE_H