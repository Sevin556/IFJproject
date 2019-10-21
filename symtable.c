/*
* Predmet  :   IFJ / IAL
* Súbor    :   symtable.c - Implementácia tabulky symbolov
* Projekt  :   Implementácia prekladača imperatívneho jazyka IFJ19
* Tým č    :   127
* Varianta :   I
* Autoři   : xhalom00, Ivan Halomi
*            xhiner00, Martin Hiner
*            xsevci64, Adam Ševčík
*            xzakji02, Jiří Žák
*/

#include "symtable.h"

/**
 * Alokuje pamäť pre uzol a nadstaví ukazatele detí na NULL
 * @param node    uzol, ktorý mí byť inicializovaný
 **/
void BTInit(tBTNodePtr* node){
        node = (tBTNodePtr*) malloc(sizeof(tBTNodePtr)); /* alokácia pamäte */

        if(node == NULL) {
                /* chyba alokácie */

                // TODO - err_call
        }

        node->lptr = NULL;
        node->rptr = NULL;

}

/**
 * a
**/
tBTNodePtr* BTInsert(char* data);
tBTNodePtr* BTSearch(char* data);
void BTCopy(tBTNodePtr*, tBTNodePtr*);
void BTDelete(tBTNodePtr*);
