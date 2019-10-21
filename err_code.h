/*
* Predmet  :   IFJ / IAL
* Súbor    :   err_code.h - Hlavičkový súbor s typmi možných chýb
* Projekt  :   Implementácia prekladača imperatívneho jazyka IFJ19
* Tým č    :   127
* Varianta :   I
* Autoři   : xhalom00, Ivan Halomi
*            xhiner00, Martin Hiner
*            xsevci64, Adam Ševčík
*            xzakji02, Jiří Žák
*/

#ifndef _ERR_CODE_H_
#define _ERR_CODE_H_

typedef enum err_ret_val {
        ERR_LEX =         1,  /* chyba lexikálnej analýzy */
        ERR_SYN =         2,  /* chyba syntaktickej analýzy */
        ERR_SEM_VAR =     3,  /* syntaktická chyba */
        ERR_RUN =         4,  /* chyba typovej kompatibility */
        ERR_SEM_FCE =     5,  /* sémantická chyba parametrov */
        ERR_SEM_OTHER =   6,  /* ostatné sémantické chyby */
        ERR_ZERO =        9,  /* delenie nulou */
        ERR_INTERN =      99, /* interná chyba prekladača */
        OK = 0
} ERR_VAL;

#endif   //_ERR_CODE_H_
