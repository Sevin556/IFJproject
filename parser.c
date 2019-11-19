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
tToken aktToken; // aktualne cteny token
tToken prevToken;
bool main = true; // TRUE kdyz jsme v mainu
tBSTNodePtr node;

//zavolas len raz niekde v maine
ERR_VAL parse() {

    symTableInit(&gTable);

    ERR_VAL rett;
    if ((rett = doParse()) != OK) {
        rett = ERR_SYN;
    }

    symTableDispose(&gTable);

    return rett;
}

ERR_VAL doParse() {      //toto budes rekurzivne volat
    ERR_VAL rett;
    aktToken = get_token();
    switch (aktToken.type) {
        case sIdentificator:
            rett = deklarace();
            if (rett != OK)
                return ERR_SYN;
        case sKeyword:
            rett = declarationKeyword();
            if (rett != OK)
                return ERR_SYN;
        case :
    }
    return OK;
}

// zjištění jeslti se jedná o proměnnou nebo o funkci
ERR_VAL deklarace() {
    ERR_VAL rett;
    prevToken = aktToken;
    aktToken = get_token();
    if()
    switch (aktToken.type) {
        case sAs:
            rett = declarationVariable();
        case sLeftBracket:
            rett = declarationFunction();
    }
}

// deklarace proměnné
ERR_VAL declarationVariable() {
    ERR_VAL rett;
    if (main) { // v mainu
        if ((symTableSearch(&gTable, prevToken.data)) != NULL)
            return ERR_SEM_VAR;
        symTableInsertVariable(&gTable, prevToken.data);
        node = symTableSearch(&gTable, prevToken.data);
    } else { // ve funkci
        if ((symTableSearch(&lTable, prevToken.data)) != NULL)
            return ERR_SEM_VAR;
        symTableInsertVariable(&lTable, prevToken.data);
        node = symTableSearch(&gTable, prevToken.data);
    }
    if (doParse() != OK)
        return ERR_LEX;
    rett = type();
    if (rett != OK)
        return rett;
    switch (aktToken.type){
        case tInteger:
            ((tVariable *)node->Data)->retType = sInteger;
            break;
        case tString:
            ((tVariable *)node->Data)->retType = sStringr;
            break;
        case tDouble:
            ((tVariable *)node->Data)->retType = sDouble;
            break;
    }
}

// typ promenne
ERR_VAL type() {
    switch (aktToken.type) {
        case tDouble:
        case tInteger:
        case tString:
            return OK;
    }
    return ERR_SYN;
}

// deklarace funkce
ERR_VAL declarationFunction() {
    ERR_VAL rett;

}

ERR_VAL declarationKeyword() {
    switch (aktToken.type) {
        case sIf:
            aktToken.type = sIf;
        case sLoop:
        case sPrint:
    }
}