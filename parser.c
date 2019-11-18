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

#include "parser.h"
#include "symtable.h"

tSymtable gTable; // globalna tabulka
tSymtable lTable; // lokalna tabulka
tToken aktToken;

//zavolas len raz niekde v maine
ERR_VAL parse() {

    symTableInit(&gTable);

    ERR_VAL rett;
    if ((rett = doParse()) != OK) {
        rett = ERR_SYN;
    } else {
        doParse();
    }

    return rett;
}

ERR_VAL doParse() {      //toto budes rekurzivne volat
    ERR_VAL rett;
    aktToken = get_token();
    switch (aktToken.type){
        case sFunction:
        case sIdentificator:
            rett = deklarace();
            if(rett != OK)
                return ERR_SYN;
    }
    return OK;
}

ERR_VAL deklarace() {
    ERR_VAL rett;
    switch (aktToken.type){
        case sIdentificator:
            rett = declarationVariable();
        case sFunction:
            rett = declarationFunction();
    }
}

ERR_VAL declarationVariable() {
    ERR_VAL rett;

}

ERR_VAL declarationFunction() {

}