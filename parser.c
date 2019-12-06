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
extern tDLListInst instList;

tToken *aktToken;               // aktualne cteny token
tToken *prevToken;              // predchazejici token
bool firstTokenOfTheLine = true; //jestli je token jako prvni
bool inMain = true;             // TRUE kdyz jsme v mainu, false ve funkci
bool argumentBool = false;      // FALSE kdyz nezpracovavam argumenty
tBSTNodePtr node;               // node na pridavani promenych nebo funkci
tBSTNodePtr gNode;              // node na hledani v globalni tabulce
string functionName;            // pomocna promenna pro zapamatovani nazvu funkce
string paramName;               // pomocna promenna pro zapamatovani promenne
int paramIndex = 0;             // pomocna promenna na parametr ve funkci
tOperand operand1;
tOperand operand2;


//zavolas len raz niekde v maine
int parse() {

    int rett;
    stringInit(&paramName);
    stringInit(&functionName);

    while (1) {
        if ((rett = doParse()) != OK) {
            return ERR_LEX;
        }
        if(aktToken->type == sEOF){
            //printf("END\n");
            break;
        }
    }

    /*stringFree(&functionName);
    stringFree(&paramName);
    stringFree(&aktToken->data);
    free(aktToken);
    stringFree(&prevToken->data);
    free(prevToken);*/

    return rett;
}

// fce na eol
int line() {
    int rett;
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

int doParse() {      //toto budes rekurzivne volat
    int rett;
    aktToken = get_token();
    if (aktToken->type == sLexError)
        return ERR_LEX;
    switch (aktToken->type) {
        case sPrint:
        case sIf:
        case sWhile:
        case sReturn:
            firstTokenOfTheLine = false;
            rett = keyWords();
            return OK;
        case sIdentificator:
            firstTokenOfTheLine = false;
            if ((rett = declaration()) != OK) {
                return ERR_SYN;
            }
            return OK;
        case sEOL:
            rett = line();
            if (rett != OK)
                return rett;
            firstTokenOfTheLine = true;
            return OK;
        case sDef:
            firstTokenOfTheLine = false;
            if ((rett = declarationFunctionHead()) != OK)
                return ERR_SYN;
            return OK;
    }
    if(firstTokenOfTheLine)
        return ERR_LEX;
    return OK;
}

// fce na rozdeleni keywordu
int keyWords() {
    int rett;
    switch (aktToken->type) {
        case sPrint:
            aktToken = get_token();
            if(aktToken->type == sLexError)
                return ERR_SYN;
            if (aktToken->subtype != sLeftBracket)
                return ERR_SYN;
            operand1 = initOperand(operand1, "", sIdentificator, true, false, false, "GF");
            instruction1op(&instList, WRITE, operand1);
            prevToken = aktToken;
            //printf("\npriiiint\n");
            while((aktToken = get_token())) {
                if (aktToken->type == sLexError)
                    return ERR_SYN;
                switch (aktToken->type) {
                    case sString:
                        operand1 = initOperand(operand1,aktToken->data.value,false,false,aktToken->type,aktToken->subtype,"GF");
                        instruction1op(&instList, WRITE, operand1);
                        break;
                    case sComma:
                        if(prevToken->subtype == sLeftBracket)
                            return ERR_SYN;
                        break;
                    case sIdentificator:
                        if(prevToken->type != sComma)
                            return ERR_LEX;
                        rett = checkVariable();
                        if(rett != OK)
                            return ERR_SYN;
                        operand1 = initOperand(operand1,aktToken->data.value,false,false,aktToken->type,aktToken->subtype,"GF");
                        instruction1op(&instList, WRITE, operand1);
                        break;
                }
                prevToken = aktToken;
                if(aktToken->subtype == sRightBracket)
                    break;
            }
            //printf("print je ok\n");
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
int declaration() {
    int rett;
    if (argumentBool == true) { // kdyz zpracovavam argumenty tak se uz muzu vratit
        prevToken = aktToken;
        rett = declarationVariable();
        return OK;
    }

    prevToken = aktToken;
    aktToken = get_token();
    if(aktToken->type == sLexError)
        return ERR_LEX;
    //printf("Hodnota prev tokenu: %s, Typ tokenu: %d\n", prevToken->data.value, prevToken->type);
    //printf("Hodnota akt tokenu: %s, Typ tokenu: %d\n", aktToken->data.value, aktToken->type);
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
int declarationVariable() {
    int rett;
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

    if (argumentBool == true)  // kdyz zpracovavam argumenty tak se uz muzu vratit
        return OK;


    aktToken=get_token();
     if(aktToken->type == sLexError)
        return ERR_LEX;

    if (aktToken->type == sIdentificator)
       {    
            tToken* temp;
            temp = get_token();
            if(temp->type == sLexError)
                return ERR_LEX;
            if (temp->type == sLeftBracket)
                /* SKONTROLUJ PARAMETRE FUNKCIE*/
            {
               rett =checkFunctionParams(aktToken);
            }
            else 
            unget_token(temp);

            rett = exprParsing(aktToken);
            if(rett != OK)
                return rett;
       }
    else 
    {
        rett = exprParsing(aktToken);
        if(rett != OK)
        return rett;
    }
    
    //printf("%d\n", rett);
    //printf("%s\n", node->Key);

    if(inMain)
        {
            operand1 = initOperand(operand1,prevToken->data.value,false,false,prevToken->type,prevToken->subtype,"GF");
            operand2 = initOperand(operand2, "tmp",false, true,sIdentificator,-1,"GF" );
        }
    else
        {
            operand1 = initOperand(operand1,prevToken->data.value,false,false,prevToken->type,prevToken->subtype,"LF");
            operand2 = initOperand(operand2, "tmp",false, true,sIdentificator,-1,"LF" );
        }
    instruction1op(&instList, POPS, operand2);
    instruction2op(&instList,MOVE,operand1,operand2);



    /*rett = typPromenne(prevToken->type);
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
    }*/

    return OK;
}

int checkVariable() {
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
int typPromenne(int type) {
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
int checkFunction() {
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
int declarationFunctionHead() {
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

int declarationFunctionBody() {
    inMain = false;
    //ERR_VAL rett;

    symTableInit(&lTable);
    gNode = symTableSearch(&gTable, functionName);


    inMain = true;
    return OK;
}

// pokud carka tak nacita dalsi parametr
int nextParametr() {
    int rett;
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

int parametr() {
    int rett;
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
