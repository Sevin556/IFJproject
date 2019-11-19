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

/* štrukturovaný dátový typ tToken */
typedef struct structToken {
        int type;             /* typ lexému */
        int subtype;          /* pomocný podtyp */
        int line;             /* riadok lexému */
        string data;          /* obsah lexému */
} tToken;


typedef enum {
        sStart,                     /* počiatočný stav */

        sEOL,                       /* návratový typ */
        sEOF,                       /* návratový typ */

        sIndent,                    /* návratový typ */
        sDedent,                    /* návratový typ */
        sDentDecide,
        sDentEnd,

        /* operandy*/
        sOperand,                   /* návratový typ */
        sMore,                      /* návratový podtyp */
        sMoreEqual,                 /* návratový podtyp */
        sLess,                      /* návratový podtyp */
        sLessEqual,                 /* návratový podtyp */
        sInequal,                   /* návratový podtyp */
        sInequalMid,
        sEqual,                     /* návratový podtyp */
        sAssignment,                /* návratový podtyp */
        sDivideFloat,               /* návratový podtyp */
        sDivideInteger,             /* návratový podtyp */
        sMultiplication,            /* návratový podtyp */
        sMinus,                     /* návratový podtyp */
        sPlus,                      /* návratový podtyp */

        /*  */
        sComma,                     /* návratový typ */
        sLeftBracket,               /* návratový typ */
        sRightBracket,              /* návratový typ */

        /* id/keyword */
        sIdentificatorOrKeyword,
        sIdentificator,             /* návratový typ */
        sDef,                       /* návratový typ */
        sElse,                      /* návratový typ */
        sIf,                        /* návratový typ */
        sNone,                      /* návratový typ */
        sPass,                      /* návratový typ */
        sReturn,                    /* návratový typ */
        sWhile,                     /* návratový typ */

        /* komentáre */
        sLineComment,
        sBlockComment,

        /* string */
        sStringStart,
        sStringRead,
        sStringEscape,
        sStringEscapeHex,
        //sStringEscapeHexValue, -redundant
        sString,                    /* návratový typ */

        /* čísla */
        sNumber,                    /* návratový typ */
        sIntegerOrDouble,
        sInteger,                   /* návratový podtyp */
        sDoublePoint,
        sDoublePointNumber,         /* návratový podtyp */
        sDoubleExponent,
        sDoubleExponentOperator,
        sDoubleExponentNumber,       /* návratový podtyp */

        /* chyba */
        sLexError = -1
} tState;


tToken* get_token(void);
tToken* init_token(void);
void assignType(tToken*);


#endif  //_SCANNER_H_
