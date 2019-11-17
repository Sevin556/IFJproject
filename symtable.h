//
// Created by zak on 09.11.19.
//

#ifndef IFJ_SYMTABLE_H
#define IFJ_SYMTABLE_H

#include <stdbool.h>
#include "string.h"

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
    int type;
} tVariable;

typedef struct function {
    int type;
    int NumOfParams;
    bool defined;
    bool declared;
    string param;
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

#endif //IFJ_SYMTABLE_H
