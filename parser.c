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

extern tSymtable gTable;               // globalna tabulka
extern tSymtable lTable;               // lokalna tabulka
tToken *aktToken;               // aktualne cteny token
tToken *prevToken;              // predchazejici token
bool inMain = true;             // TRUE kdyz jsme v mainu, false ve funkci
bool argumentBool = false;      // FALSE kdyz nezpracovavam argumenty
bool printBool = false;         // FALSE kdyz nejsem v printu
tBSTNodePtr node;               // node na pridavani promenych nebo funkci
tBSTNodePtr gNode;              // node na hledani v globalni tabulce
string functionName;            // pomocna promenna pro zapamatovani nazvu funkce
string paramName;               // pomocna promenna pro zapamatovani promenne
int paramIndex = 0;             // pomocna promenna na parametr ve funkci

//zavolas len raz niekde v maine
ERR_VAL parse() {

    ERR_VAL rett;
    stringInit(&paramName);
    stringInit(&functionName);

    while (1) {
        if ((rett = doParse()) != OK) {
            return ERR_LEX;
        }
        if(aktToken->type == sEOF)
            break;
    }

    stringFree(&functionName);
    stringFree(&paramName);
    stringFree(&aktToken->data);
    free(aktToken);
    stringFree(&prevToken->data);
    free(prevToken);

    return rett;
}

// fce na eol
ERR_VAL line() {
    ERR_VAL rett;
    switch (aktToken->type) {
        case sEOL:
            if (doParse() != OK)
                return ERR_LEX;
            rett = line();
            return rett;
        default:
            rett = OK;
            return rett;
    }
}

ERR_VAL doParse() {      //toto budes rekurzivne volat
    ERR_VAL rett;
    aktToken = get_token();
    if (aktToken->type == sLexError)
        return ERR_LEX;
    switch (aktToken->type) {
        case sPrint:
        case sIf:
        case sWhile:
        case sReturn:
            rett = keyWords();
            return OK;
        case sIdentificator:
            if ((rett = declaration()) != OK) {
                return ERR_SYN;
            }
            return OK;
        case sEOL:
            rett = line();
            if (rett != OK)
                return rett;
            return OK;
        case sDef:
            if ((rett = declarationFunctionHead()) != OK)
                return ERR_SYN;
            return OK;
    }
    return OK;
}

// fce na rozdeleni keywordu
ERR_VAL keyWords() {
    //ERR_VAL rett;
    switch (aktToken->type) {
        case sPrint:
            if (doParse() != OK)
                return ERR_LEX;
            if (aktToken->subtype != sLeftBracket)
                return ERR_SYN;
            if (doParse() != OK)
                return ERR_LEX;
            switch (aktToken->type) {
                case sString:
                case sComma:
                case sIdentificator:
                    printBool = true;

                    printBool = false;
                    break;
                case sRightBracket:
                    return OK;
            }

            // call gen instruction
            return OK;
        case sIf:
            //call expression
            return OK;
        case sWhile:
            //call expression
            return OK;
        case sReturn:
            if (inMain)
                return ERR_SEM_FCE;
            // call instruction gen return with parametr
            return OK;
    }
    return OK;
}

// zjištění jeslti se jedná o proměnnou nebo o funkci nebo argument
ERR_VAL declaration() {
    ERR_VAL rett;
    if (argumentBool == true) { // kdyz zpracovavam argumenty tak se uz muzu vratit
        prevToken = aktToken;
        rett = declarationVariable();
        return OK;
    } else if (printBool == true) { // kdyz jsem v printu
        rett = checkVariable();
        if (rett != OK)
            return ERR_SEM_FCE;
    }

    prevToken = aktToken;
    aktToken = get_token();
    printf("Hodnota tokenu: %s, Typ tokenu: %d\n", prevToken->data.value, prevToken->type);
    printf("Hodnota tokenu: %s, Typ tokenu: %d\n", aktToken->data.value, aktToken->type);
    switch (aktToken->type) {
        case sAssignment:
            rett = declarationVariable();
            return rett;
        case sLeftBracket:
            rett = checkFunction();
            return rett;
    }
    return ERR_SYN;
}

// deklarace proměnné
ERR_VAL declarationVariable() {
    ERR_VAL rett;
    if (inMain) { // v mainu
        if ((symTableSearch(&gTable, prevToken->data)) != NULL)
            return ERR_SEM_VAR;
        symTableInsertVariable(&gTable, prevToken->data);
        node = symTableSearch(&gTable, prevToken->data);
    } else { // ve funkci
        if ((symTableSearch(&lTable, prevToken->data)) != NULL)
            return ERR_SEM_VAR;
        symTableInsertVariable(&lTable, prevToken->data);
        node = symTableSearch(&gTable, prevToken->data);
    }

    //rett = exprParing();
    printf("%s\n", node->Key);

    if (argumentBool == true)  // kdyz zpracovavam argumenty tak se uz muzu vratit
        return OK;

    if (doParse() != OK)
        return ERR_LEX;
    prevToken = aktToken;
    if (doParse() != OK)
        return ERR_LEX;
    // kontrola, jestli za prirazenim nahodou jeste neco neni (a = 10 +5)
    if (aktToken->type != sEOL || aktToken->type != sLeftBracket)
        //call expression
        rett = typPromenne(prevToken->type);
    if (rett != OK)
        return rett;
    switch (prevToken->type) {
        case sInputI:
        case sInteger:
            ((tVariable *) node->Data)->retType = sInteger;
            break;
        case sInputS:
        case sString:
            ((tVariable *) node->Data)->retType = sString;
            break;
        case sInputF:
        case sDoublePointNumber:
            ((tVariable *) node->Data)->retType = sDoublePointNumber;
            break;
    }
    printf("Konec variable\n");

    return OK;
}

ERR_VAL checkVariable() {
    if (aktToken->type == sIdentificator) {
        if (inMain == true) {
            node = symTableSearch(&gTable, aktToken->data);
            if (node == NULL)
                return ERR_SEM_FCE;
        } else {
            node = symTableSearch(&lTable, aktToken->data);
            if (node == NULL)
                return ERR_SEM_FCE;
        }
        return OK;
    }
    return ERR_SYN;
}

// typ promenne
ERR_VAL typPromenne(int type) {
    switch (type) {
        case sInputI:
        case sInputF:
        case sInputS:
        case sDoublePointNumber:
        case sInteger:
        case sString:
            return OK;
    }
    return ERR_SYN;
}

// kontrola, jeslti bylla uz funkce definovana
ERR_VAL checkFunction() {
    if (symTableSearch(&gTable, prevToken->data) == NULL) {
        symTableInsertFunction(&gTable, prevToken->data);
    } else {
        node = symTableSearch(&gTable, prevToken->data);
    }
    if (doParse() != OK)
        return ERR_LEX;
    /*while () {
        if (aktToken->type != sIdentificator)
            return ERR_SEM_FCE;
        // dodelat kontroli jestli to bylo uz definovane nebo jeslti se to ma pridat a pak smazat
    }*/

    return OK;
}

// deklarace funkce
ERR_VAL declarationFunctionHead() {
    ERR_VAL rett;
    if ((aktToken = get_token()) != OK)
        return ERR_LEX;
    if (aktToken->type != sIdentificator)
        return ERR_SEM_VAR;
    functionName = aktToken->data;
    //kontrola, jestli už nebyla definovana
    if (symTableSearch(&gTable, aktToken->data) != NULL) {
        return ERR_SEM_FCE;
    } else {
        symTableInsertFunction(&gTable, aktToken->data);
        node = symTableSearch(&gTable, aktToken->data);
    }
    if (doParse() != OK)
        return ERR_LEX;
    if (aktToken->type != sLeftBracket)
        return ERR_SEM_VAR;
    rett = nextParametr();

    rett = declarationFunctionBody();
    return rett;
}

ERR_VAL declarationFunctionBody() {
    inMain = false;
    //ERR_VAL rett;

    symTableInit(&lTable);
    gNode = symTableSearch(&gTable, functionName);


    inMain = true;
    return OK;
}

// pokud carka tak nacita dalsi parametr
ERR_VAL nextParametr() {
    ERR_VAL rett;
    argumentBool = true;
    switch (aktToken->type) {
        case sLeftBracket:
        case sComma:
            if (doParse() != OK)
                return ERR_LEX;
            rett = parametr();
            if (rett != OK)
                return rett;
            return nextParametr();
        case sRightBracket:
            argumentBool = false;
            return OK;
    }
    return ERR_SYN;
}

ERR_VAL parametr() {
    ERR_VAL rett;
    switch (aktToken->type) {
        case sIdentificator:
            paramName = aktToken->data;
            for (int i = 0; i < paramIndex; i++) {
                if (stringCompare(&((tFunction *) node->Data)->paramName[i], &paramName) == true)
                    return ERR_SEM_FCE;
            }
            if (symTableSearch(&gTable, paramName) != NULL)
                return ERR_SEM_FCE;
            ((tFunction *) node->Data)->paramName[paramIndex] = paramName;

            paramIndex++;
            if (doParse() != OK)
                return ERR_LEX;
            rett = nextParametr();
            if (rett != OK)
                return rett;
            return OK;
        case sRightBracket:
            paramIndex = 0;
            argumentBool = false;
            return OK;
    }
    return OK;
}
