/*
* Predmet  :   IFJ / IAL
* Súbor    :   parser.h - Syntaktický a semantický analyzátor
* Projekt  :   Implementácia prekladača imperatívneho jazyka IFJ19
* Tým č    :   127
* Varianta :   I
* Autoři   : xhalom00, Ivan Halomi
*            xhiner00, Martin Hiner
*            xsevci64, Adam Ševčík
*            xzakji02, Jiří Žák
*/

#ifndef IFJ_EXPRPARSER_H
#define IFJ_EXPRPARSER_H

#include <stdio.h>

#include <stdbool.h>
#include <stdlib.h>
//#include "parser.h"
#include "symtable.h"
#include "scanner.h"
//#include "stack.h"
#include "string.h"
#include "lifo.h"


typedef enum
{
	H,
	L,
	EQ,
	ERROR,
	NONE,
	EXITPARSE
}exprTable;

int exprParsing(tToken*);
int ApplyRule(tStack*);
int shiftToStack (tStack*,tToken*);
tRedukToken* createNewToken(tToken*);
int checkOperator(tStack* ,int);
int checkSemantic(tRedukToken* ,tRedukToken* , int );
int reduceBrackets(tStack*);
#endif //IFJ_EXPRPARSER_H