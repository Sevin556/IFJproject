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

typedef enum {
    sIf,
    sElse,
    sLoop,
    sPrint,
    sInput,
    sReturn,

    tInteger,
    tDouble,
    tString
}keyword_type;

int parse();
int prog();

#endif //IFJ_PARSER_H
