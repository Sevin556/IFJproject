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

/* štrukturovaný typ tToken */
typedef struct {
  int type;         /* typ lexému */
  string lexeme;    /* obsah lexémy */
}tToken;



typedef enum {
  tStart
} tState;

#endif  //_SCANNER_H_
