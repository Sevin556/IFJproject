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

ERR_VAL parse();

ERR_VAL doParse();

ERR_VAL line();

ERR_VAL declaration();

ERR_VAL declarationVariable();

ERR_VAL declarationFunctionHead();

ERR_VAL declarationFunctionBody();

ERR_VAL checkFunction();

ERR_VAL declarationKeyword();

ERR_VAL typPromenne();

ERR_VAL parametr();

ERR_VAL nextParametr();

#endif //IFJ_PARSER_H
