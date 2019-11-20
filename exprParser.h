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
#include "scanner.h"
#include <stdbool.h>
#include "lifo.h"

typedef struct
{
	tToken *Token;
	exprTable  priorita;
	bool Redukovany;
	bool zeroflag;
}tRedukToken;

typedef enum
{
	H,
	L,
	EQ,
	ERROR,
	NONE,
	EXITPARSE
}exprTable;


zjednodus(*tStack,*tStack,*bool);
#endif //IFJ_EXPRPARSER_H