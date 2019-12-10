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

#ifndef IFJ_PARSER_H
#define IFJ_PARSER_H

#include <stdio.h>
#include "scanner.h"
#include "string.h"
#include "err_code.h"
#include "symtable.h"
#include "exprParser.h"
#include "functionParser.h"
#include "instruction-list.h"

int parse();

int doParse();

int line();

int keyWords();

int declaration();

int declarationVariable();

int checkVariable();

int declarationFunctionHead();

int declarationFunctionBody();

int checkFunction();

int declarationKeyword();

int typPromenne();

int parametr();

int nextParametr();

//nacita parametre funkcie
int loadParams();

#endif //IFJ_PARSER_H
