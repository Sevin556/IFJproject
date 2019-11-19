/*
* Predmet  :   IFJ / IAL
* Súbor    :   scanner.c - Implementácia lexikálneho analyzátora
* Projekt  :   Implementácia prekladača imperatívneho jazyka IFJ19
* Tým č    :   127
* Varianta :   I
* Autoři   : xhalom00, Ivan Halomi
*            xhiner00, Martin Hiner
*            xsevci64, Adam Ševčík
*            xzakji02, Jiří Žák
*/

#ifndef _SCANNER_H_
#define _SCANNER_H_

#include "string.h"

/* štrukturovaný typ tToken */
typedef struct {
    int type;         /* typ lexému */
    string data;    /* obsah lexémy */
} tToken;

typedef enum {
    sStart,             /* počiatočný stav */

    sEOL,
    sEOF,

    sIndent,
    sDedent,
    sDentDecide,
    sDentEnd,

    sMore,              /*  */
    sMoreEqual,
    sLess,
    sLessEqual,
    sInequal,
    sInequalMid,
    sEqual,
    sAssignment,
    sDivideFloat,
    sDivideInteger,
    sMultiplication,
    sMinus,
    sPlus,

    sSemicolon,
    sComma,
    sLeftBracket,
    sRightBracket,

    sIdentificatorOrKeyword,
    sIdentificator,
    sKeyword,

    sLineComment,
    sBlockComment,

    sStringStart,
    sStringRead,
    sStringEscape,
    sStringEscapeHex,
    sStringEscapeHexValue,
    sString,

    sIntegerOrDouble,
    sInteger,
    sDoublePoint,
    sDoublePointNumber,
    sDoubleExponent,
    sDoubleExponentOperator,
    sDoubleExponentNumber

} tState;

tToken get_token(void);

#endif  //_SCANNER_H_