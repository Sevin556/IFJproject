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
extern tDLListInst funcList;

tToken *aktToken;               // aktualne cteny token
tToken *prevToken;              // predchazejici token
bool inMain = true;             // TRUE kdyz jsme v mainu, false ve funkci
bool inFunction= false;      // FALSE kdyz nezpracovavam argumenty
tBSTNodePtr node;               // node na pridavani promenych nebo funkci
tBSTNodePtr gNode;              // node na hledani v globalni tabulce
string functionName;            // pomocna promenna pro zapamatovani nazvu funkce
int paramIndex = 0;             // pomocna promenna na parametr ve funkci
tOperand operand1;
tOperand operand2;
tOperand operand3;

int keyWordNumber = 0;

//zavolas len raz niekde v maine
int parse() {

    int rett = OK;


    operand1 = initOperand(operand1, "$main", true, false, sIdentificator, -1, "");
    instruction1op(&funcList, JUMP, operand1);
    instruction1op(&instList,LABEL,operand1);
    instruction0op(&instList, CREATEFRAME);
    instruction0op(&instList,PUSHFRAME);
    //tmp a tmp2 vyuziva pretypovanie a podmienky
    operand2 = initOperand(operand2, "tmp", false, true, sIdentificator, -1, "GF");
    instruction1op(&instList, DEFVAR, operand2);
    operand2 = initOperand(operand2, "tmp2", false, true, sIdentificator, -1, "GF");
    instruction1op(&instList, DEFVAR, operand2);
    if ((rett = doParse()) != OK) {
        return rett;
    }
    if (aktToken->type != sEOF) {
        return ERR_SYN;
    }

    free(aktToken);
    free(prevToken);

    return rett;
}


int doParse() {      //toto budes rekurzivne volat
    int rett = OK;

    while (1) {
        aktToken = get_token();
        if (aktToken->type == sLexError) {
            return ERR_LEX;
        }

        switch (aktToken->type) {
            case sPrint:
            case sIf:
            case sWhile:
            case sReturn:
                rett = keyWords();
                if (rett != OK)
                    return rett;

                break;
            case sIdentificator:
                if ((rett = declaration()) != OK) {
                    return rett;
                }

                break;
            case sEOL:
                break;
            case sInputS:
            case sInputI:
            case sInputF:
            case sLen:
            case sSubstr:
            case sOrd:
            case sChr:
                prevToken = aktToken;
                aktToken = get_token();
                if (aktToken->type == sLexError) {
                    return ERR_LEX;
                }
                node = symTableSearch(&gTable, prevToken->data);
                if (node == NULL)
                    return ERR_SEM_VAR;
                rett = checkFunctionParams(prevToken, ((tFunction *) node->Data)->paramCounter);
                if (rett != OK) {
                    return rett;
                }

                if (inFunction){
                    //instruction0op(&funcList, PUSHFRAME);
                    instruction0op(&funcList, CREATEFRAME);
                    operand1 = initOperand(operand1,prevToken->data.value,true,false,sIdentificator,-1,"GF");
                    instruction1op(&funcList,CALL,operand1);
                } else
                {
                    //instruction0op(&instList, PUSHFRAME);
                    instruction0op(&instList, CREATEFRAME);
                    operand1 = initOperand(operand1,prevToken->data.value,true,false,sIdentificator,-1,"GF");
                    instruction1op(&instList,CALL,operand1);
                }
                
                
                
                aktToken = get_token();
                if (aktToken->type == sLexError) {
                    return ERR_LEX;
                }
                if (aktToken->subtype != sRightBracket) {
                    return ERR_SYN;
                }
                break;

            case sDef:

                rett = declarationFunctionHead();
                if (rett != OK) {
                    return rett;
                }

                break;
            case sDedent:
                /* vracia špeciálnu hodnotu pre kontrolu potreby dedentu */
                return sDedent;

            case sEOF:
                return OK;
            case sPass:
                break;
            default:
                return ERR_SYN;
        }
    }

}

// fce na rozdeleni keywordu
int keyWords() {
    int rett = OK;

    switch (aktToken->type) {
        case sPrint:
            aktToken = get_token();
            if (aktToken->type == sLexError)
                return ERR_LEX;
            if (aktToken->subtype != sLeftBracket)
                return ERR_SYN;
            prevToken = aktToken;
            while ((aktToken = get_token())) {
                if (aktToken->type == sLexError)
                    return ERR_LEX;
                switch (aktToken->type) {
                    case sString:
                        if (inMain) {
                            operand1 = initOperand(operand1, aktToken->data.value, false, false, aktToken->type,
                                                   aktToken->subtype, "GF");
                        } else {
                            operand1 = initOperand(operand1, aktToken->data.value, false, false, aktToken->type,
                                                   aktToken->subtype, "LF");
                        }
                        if (inFunction){
                            instruction1op(&funcList, WRITE, operand1);
                        } else
                        {
                            instruction1op(&instList, WRITE, operand1);
                        }
                        
                        
                        break;
                    case sComma:
                        if (prevToken->subtype == sLeftBracket)
                            return ERR_SYN;
                        break;
                    case sIdentificator:
                        if (prevToken->type != sComma){
                            if(prevToken->subtype != sLeftBracket)
                                return ERR_SYN;
                        }
                        rett = checkVariable();
                        if (rett != OK)
                            return rett;
                        if (inMain) {
                            operand1 = initOperand(operand1, aktToken->data.value, false, false, aktToken->type,
                                                   aktToken->subtype, "GF");
                        } else {
                            operand1 = initOperand(operand1, aktToken->data.value, false, false, aktToken->type,
                                                   aktToken->subtype, "LF");
                        }

                        if (inFunction){
                            instruction1op(&instList, WRITE, operand1);
                        } else
                        {
                            instruction1op(&instList, WRITE, operand1);
                        }
                        
                        
                        break;
                }
                prevToken = aktToken;
                if (aktToken->subtype == sRightBracket)
                    break;
            }

            return OK;
        case sIf:
            aktToken = get_token();
            if (aktToken->type == sLexError) {
                return ERR_LEX;
            }

            /* VYTVORENIE DOCASNEJ PREMENNEJ NA POROVNANIE VYSLEDKU*/
            operand1 = initOperand(operand1, "tmp", false, true, sIdentificator, -1, "GF");

            rett = exprParsing(aktToken);
            if (rett != OK) {
                return rett;
            }
            if (inFunction)
            {
                instruction1op(&funcList, POPS, operand1);
            } else
            {
                instruction1op(&instList, POPS, operand1);
            }
            
            

            char labelElse[10];
            sprintf(labelElse, "else%d", keyWordNumber++);

            operand2 = initOperand(operand2, "true", false, true, -1, sBool, "TF");

            //instruction2op(&instList,MOVE,operand1,operand2);
            //operand2=initOperand(operand2,"true",false,true,sIdentificator,-1,"TF");
            tOperand operandElse = initOperand(operand3, labelElse, true, false, sIdentificator, -1, "");
            if (inFunction){
                instruction3op(&funcList, JUMPIFNEQ, operandElse, operand1, operand2);
                //instruction0op(&funcList, PUSHFRAME);
                instruction0op(&funcList, CREATEFRAME);
                instruction0op(&funcList,PUSHFRAME);
            } else
            {
                instruction3op(&instList, JUMPIFNEQ, operandElse, operand1, operand2);
                //instruction0op(&instList, PUSHFRAME);
                instruction0op(&instList, CREATEFRAME);
                instruction0op(&instList,PUSHFRAME);
            }
            

            inMain = false;
            aktToken = get_token();
            if (aktToken->type == sLexError) {
                return ERR_LEX;
            }

            if (aktToken->type != sEOL)
                return ERR_SYN;
            /****************zacina telo ifu************************/

            aktToken = get_token();
            if (aktToken->type == sLexError) {
                return ERR_LEX;
            }

            if (aktToken->type != sIndent)
                return ERR_SYN;

            rett = doParse();
            if (rett != sDedent) {
                return ERR_SYN;
            } else {
                rett = OK;
            }
            char labelEnd[10];
            sprintf(labelEnd, "endif%d", keyWordNumber++);
            tOperand operandEndIF = initOperand(operand2, labelEnd, true, false, sIdentificator, -1, "");
            if (inFunction)
            {
                instruction1op(&funcList, JUMP, operandEndIF);
            } else
            {
                instruction1op(&instList, JUMP, operandEndIF);
            }
            
            
            

            /***********************nasleduje else************************ */
            aktToken = get_token();
            if (aktToken->type == sLexError) {
                return ERR_LEX;
            }

            if (aktToken->type != sElse) {
                return ERR_SYN;
            }


            aktToken = get_token();
            if (aktToken->type == sLexError) {
                return ERR_LEX;
            }
            if (aktToken->type != sColon)
                return ERR_SYN;


            aktToken = get_token();
            if (aktToken->type == sLexError) {
                return ERR_LEX;
            }
            if (aktToken->type != sEOL)
                return ERR_SYN;

            aktToken = get_token();
            if (aktToken->type == sLexError) {
                return ERR_LEX;
            }
            if (aktToken->type != sIndent)
                return ERR_SYN;

            //skok do ELSE
            //operand3 = initOperand(operand3, labelElse, true, false, sIdentificator, -1, ""); 
            if (inFunction){
                instruction1op(&funcList, LABEL, operandElse);
            } else
            {
                instruction1op(&instList, LABEL, operandElse);
            }
            
            


            rett = doParse();
            if (rett != sDedent) {
                return ERR_SYN;
            } else {
                rett = OK;
            }
            //skok po vykonani IF
            operand2 = initOperand(operand2, labelEnd, true, false, sIdentificator, -1, "");
            if (inFunction){
                instruction1op(&funcList, LABEL, operandEndIF);
                instruction0op(&funcList, POPFRAME);
            } else
            {
                instruction1op(&instList, LABEL, operandEndIF);
                instruction0op(&instList, POPFRAME);
            }
            
            
            inMain = true;
            return OK;
        case sWhile:

            aktToken = get_token();
            if (aktToken->type == sLexError)
                return ERR_LEX;

            char labelWhile[10];
            sprintf(labelWhile, "while%d", keyWordNumber);
            char endWhile[10];
            sprintf(endWhile, "endwhile%d", keyWordNumber);

            operand1 = initOperand(operand1, labelWhile, true, false, sIdentificator,
                                   aktToken->subtype, "");
            if (inFunction){
                instruction1op(&funcList, LABEL, operand1);
            } else
            {
                instruction1op(&instList, LABEL, operand1);
            }
            
            
            operand2 = initOperand(operand2, endWhile, true, false, sIdentificator,
                                   aktToken->subtype, "");



            rett = exprParsing(aktToken);
            if (rett != OK){
                return rett;
            }

            operand1 = initOperand(operand1, "tmp", false, true, sIdentificator,
                                   aktToken->subtype, "GF");
            
            if (inFunction){
                instruction1op(&funcList,POPS,operand1);
            } else
            {
                instruction1op(&instList,POPS,operand1);
            }
            
            
            operand3 = initOperand(operand3, "true", false, true, -1,
                                   sBool, "");
            if (inFunction){
                instruction3op(&funcList, JUMPIFNEQ, operand2,operand1,operand3);
                instruction0op(&funcList,PUSHFRAME);
                instruction0op(&funcList,CREATEFRAME);
            } else
            {
                instruction3op(&instList, JUMPIFNEQ, operand2,operand1,operand3);
                instruction0op(&instList,PUSHFRAME);
                instruction0op(&instList,CREATEFRAME);
            }
            


            aktToken = get_token();
            if (aktToken->type == sLexError)
                return ERR_LEX;
            if (aktToken->type != sEOL)
                return ERR_SYN;
            aktToken = get_token();
            if (aktToken->type == sLexError)
                return ERR_LEX;
            if (aktToken->type != sIndent)
                return ERR_SYN;

            inMain = false;
            /****************TELO WHILE****************/
            rett = doParse();
            if(rett != sDedent)
                return ERR_SYN;
            if(rett == sDedent)
                rett = OK;

            inMain = true;
            //jump na zaciatok while
            operand1 = initOperand(operand1, labelWhile, true, false, sIdentificator,
                                   aktToken->subtype, "");
            if (inFunction){
                instruction1op(&funcList,JUMP,operand1);
            } else
            {
                instruction1op(&instList,JUMP,operand1);
            }
            

            //LABEL s koncom WHILE
            operand1 = initOperand(operand1, endWhile, true, false, sIdentificator,
                                   aktToken->subtype, "");

            if (inFunction){
                instruction1op(&funcList,LABEL,operand1);
                keyWordNumber++;
                instruction0op(&funcList,POPFRAME);
            } else
            {
                instruction1op(&instList,LABEL,operand1);
                keyWordNumber++;
                instruction0op(&instList,POPFRAME);
            }
            
            
            return rett;
        case sReturn:
            if (inMain) {
                return ERR_SEM_FCE;
            }
            // call instruction gen return with parametr
            aktToken=get_token();
            if (aktToken->type == sLexError) {
                return ERR_LEX;
            }
            if (aktToken->type == sEOL){
                operand1 = initOperand(operand1,"nil",false,true,-1,sNone,"GF");
                if (inFunction){
                    instruction1op(&funcList,PUSHS,operand1);
                } else
                {
                    instruction1op(&instList,PUSHS,operand1);
                }
                
                
            }
            else {
                operand1 = initOperand(operand1,aktToken->data.value,false,true,aktToken->type,aktToken->subtype,"TF");
                if (inFunction){
                    instruction1op(&funcList,PUSHS,operand1);
                } else
                {
                    instruction1op(&instList,PUSHS,operand1);
                }
                
                
            }

            if (inFunction){
                instruction0op(&funcList, RETURN);
            } else
            {
                instruction0op(&instList, RETURN);
            }
            
            
            
            return OK;
    }
    return OK;
}

// zjištění jeslti se jedná o proměnnou nebo o funkci nebo argument
int declaration() {
    int rett = OK;

    prevToken = aktToken;
    aktToken = get_token();
    if (aktToken->type == sLexError) {
        return ERR_LEX;
    }

    //printf("Hodnota prev tokenu: %s, Typ tokenu: %d\n", prevToken->data.value, prevToken->type);
    //printf("Hodnota akt tokenu: %s, Typ tokenu: %d\n", aktToken->data.value, aktToken->type);
    if (aktToken->type == sAssignment) {
        rett = declarationVariable();
        return rett;
    }
    if (aktToken->subtype == sLeftBracket) {
        node = symTableSearch(&gTable, prevToken->data);
        if (node == NULL)
            return ERR_SEM_VAR;
        rett = checkFunctionParams(prevToken,((tFunction *) node->Data)->paramCounter);
        if (rett != OK) {
            return rett;
        }

        if (inFunction){
            //instruction0op(&funcList, PUSHFRAME);
            instruction0op(&funcList, CREATEFRAME);
            operand1 = initOperand(operand1,prevToken->data.value,true,false,sIdentificator,-1,"GF");
            instruction1op(&funcList,CALL,operand1);
        } else
        {
            //instruction0op(&instList, PUSHFRAME);
            instruction0op(&instList, CREATEFRAME);
            operand1 = initOperand(operand1,prevToken->data.value,true,false,sIdentificator,-1,"GF");
            instruction1op(&instList,CALL,operand1);
        }
        

        
        aktToken = get_token();
        if (aktToken->type == sLexError) {
            return ERR_LEX;
        }
        if (aktToken->subtype != sRightBracket) {
            return ERR_SYN;
        }
    }

    /*
 if (aktToken->subtype == sLeftBracket){
     rett = checkFunction();
     return rett;
 }*/



    return ERR_SYN;
}

// deklarace proměnné
int declarationVariable() {
    int rett = OK;

    if (inMain) { // v mainu
        if ((symTableSearch(&gTable, prevToken->data)) == NULL) {
            symTableInsertVariable(&gTable, prevToken->data);
            operand1 = initOperand(operand1, prevToken->data.value, false, false, sIdentificator, prevToken->subtype,
                                   "GF");
            if (inFunction){
                instruction1op(&funcList, DEFVAR, operand1);
            } else
            {
                instruction1op(&instList, DEFVAR, operand1);
            }
            
            
        }

        node = symTableSearch(&gTable, prevToken->data);
    } else { // ve funkci
        if ((symTableSearch(&lTable, prevToken->data)) == NULL) {
            symTableInsertVariable(&lTable, prevToken->data);
            operand1 = initOperand(operand1, prevToken->data.value, false, false, sIdentificator, prevToken->subtype,
                                   "LF");

            if (inFunction){
                instruction1op(&funcList, DEFVAR, operand1);
            } else
            {
                instruction1op(&instList, DEFVAR, operand1);
            }
            
            
        }

        node = symTableSearch(&lTable, prevToken->data);

    }


    aktToken = get_token();
    if (aktToken->type == sLexError) {
        return ERR_LEX;
    }

    if (aktToken->type == sIdentificator) {
        tToken *temp;
        temp = get_token();

        if (temp->type == sLexError) {
            return ERR_LEX;
        }

        if (temp->subtype == sLeftBracket) {
            /* SKONTROLUJ PARAMETRE FUNKCIE*/

            if (symTableSearch(&gTable, aktToken->data) == NULL)
                return ERR_SEM_VAR;
            rett = checkFunctionParams(aktToken, ((tFunction *) node->Data)->paramCounter);
            if (rett != OK)
                return rett;
            
            if (inFunction){
                //instruction0op(&funcList, PUSHFRAME);
                instruction0op(&funcList, CREATEFRAME);
                operand1 = initOperand(operand1,aktToken->data.value,true,false,sIdentificator,-1,"GF");
                instruction1op(&funcList,CALL,operand1);
            } else
            {
                //instruction0op(&instList, PUSHFRAME);
                instruction0op(&instList, CREATEFRAME);
                operand1 = initOperand(operand1,aktToken->data.value,true,false,sIdentificator,-1,"GF");
                instruction1op(&instList,CALL,operand1);
            }
            


        } else {
            unget_token(temp);
            rett = exprParsing(aktToken);
            if (rett != OK)
                return rett;
        }

    } else {
        rett = exprParsing(aktToken);
        if (rett != OK) {
            return rett;
        }
    }

    //printf("%d\n", rett);
    //printf("%s\n", node->Key);

    if (inMain) {
        operand1 = initOperand(operand1, prevToken->data.value, false, false, prevToken->type, prevToken->subtype,
                               "GF");
    } else {
        operand1 = initOperand(operand1, prevToken->data.value, false, false, prevToken->type, prevToken->subtype,
                               "LF");
    }
    operand2 = initOperand(operand2, "tmp", false, true, sIdentificator, -1, "GF");
    if (inFunction)
    {
        instruction1op(&funcList, POPS, operand2);
        instruction2op(&funcList, MOVE, operand1, operand2);
    } else
    {
        instruction1op(&instList, POPS, operand2);
        instruction2op(&instList, MOVE, operand1, operand2);
    }
    
    
    



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
            if (node == NULL) {
                return ERR_SEM_VAR;
            }
        } else {
            node = symTableSearch(&lTable, aktToken->data);
            if (node == NULL) {
                return ERR_SEM_VAR;
            }
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

// kontrola, jeslti bylla uz funkce definovana vracia ERR_SEM_VAR ak neexistuje inak je node dana funkcia
int checkFunction() {

    if (symTableSearch(&gTable, prevToken->data) == NULL) {
        return ERR_SEM_VAR;
    } else {
        node = symTableSearch(&gTable, prevToken->data);
    }

    return OK;
}

// deklarace funkce
int declarationFunctionHead() {
    int rett = OK;
    aktToken = get_token();
    if (aktToken->type == sLexError) {
        return ERR_LEX;
    }
    if (aktToken->type != sIdentificator) {
        return ERR_SYN;
    }
    inFunction= true;
    functionName = aktToken->data;
    //kontrola, jestli už nebyla definovana
    if (symTableSearch(&gTable, aktToken->data) != NULL) {
        return ERR_SEM_VAR;
    } else {
        symTableInsertFunction(&gTable, aktToken->data);
        node = symTableSearch(&gTable, aktToken->data);
        operand1 = initOperand(operand1, aktToken->data.value, true, false, sIdentificator, aktToken->subtype, "GF");
        if (inFunction){
            instruction1op(&funcList, LABEL, operand1);
        } else
        {
            instruction1op(&instList, LABEL, operand1);
        }
        
        
    }

    prevToken = aktToken;
    aktToken = get_token();
    if (aktToken->type == sLexError) {
        return ERR_LEX;
    }


    if (aktToken->subtype != sLeftBracket) {
        return ERR_SYN;
    }

    rett = loadParams(functionName);


    rett = declarationFunctionBody();
    inFunction=false;
    return rett;
}

int declarationFunctionBody() {
    inMain = false;
    //ERR_VAL rett;

    symTableInit(&lTable);
    gNode = symTableSearch(&gTable, functionName);
    int ret = OK;
    aktToken = get_token();
    if (aktToken->type == sLexError)
        return ERR_LEX;
    if (aktToken->type != sColon)
        return ERR_SYN;


    aktToken = get_token();
    if (aktToken->type == sLexError)
        return ERR_LEX;


    if (aktToken->type != sEOL)
        return ERR_SYN;

    aktToken = get_token();
    if (aktToken->type == sLexError)
        return ERR_LEX;

    if (aktToken->type != sIndent)
        return ERR_SYN;

    ret = doParse();
    if (ret != sDedent)
        return ret;
    else {
        ret = OK;
    }
    inMain = true;
    return ret;
}


int loadParams() {

    bool ciarka = false;//ID musi mat ciarku pred sebou
    bool isFirst = true;// ak je prvy nemusi mat ciarku pred sebou
    tBSTNodePtr IDFunkcie = symTableSearch(&gTable, functionName);

    while (1) {
        aktToken = get_token();
        if (aktToken->type == sLexError)
            return ERR_LEX;


        if (aktToken->type == sIdentificator) {
            if (isFirst) {
                isFirst = false;
                ciarka = false;
                //zvys pocet parametrov do symtable
                ((tFunction *) IDFunkcie->Data)->paramName[((tFunction *) IDFunkcie->Data)->paramCounter] = aktToken->data;
                (((tFunction *) IDFunkcie->Data)->paramCounter)++;
                symTableInsertVariable(&lTable, aktToken->data);
            } else {
                if (ciarka) {
                    //zvys pocet parametrov
                    ((tFunction *) IDFunkcie->Data)->paramName[((tFunction *) IDFunkcie->Data)->paramCounter] = aktToken->data;
                    (((tFunction *) IDFunkcie->Data)->paramCounter)++;
                    symTableInsertVariable(&lTable, aktToken->data);
                    ciarka = false;
                } else {
                    return ERR_SYN;
                }
            }
        } else if (aktToken->type == sComma) {
            if (ciarka) {//dve ciarky po sebe
                return ERR_SYN;
            } else {
                ciarka = true;
            }
        } else if (aktToken->subtype == sRightBracket) {
            if (ciarka) {
                return ERR_SYN;
            } else {
                return OK;
            }
        } else {
            return ERR_SYN;
        }
    }
}