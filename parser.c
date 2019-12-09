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

    int rett = OK;
    stringInit(&paramName);
    stringInit(&functionName);


        if ((rett = doParse()) != OK) {
            return rett;
        }
        if(aktToken->type != sEOF){
            //printf("END\n");
        return ERR_SYN;
    }

    /*stringFree(&functionName);
    stringFree(&paramName);
    stringFree(&aktToken->data);
    free(aktToken);
    stringFree(&prevToken->data);
    free(prevToken);*/

    return rett;
}



int doParse() {      //toto budes rekurzivne volat
    int rett= OK;
    
    while (1)
    {
        aktToken = get_token();
        if (aktToken->type == sLexError) {
            return ERR_LEX;
        }
        
       switch (aktToken->type) {
        case sPrint:
        case sIf:
        case sWhile:
        case sReturn:
            //ak neni prvy na riadku tak je to chyba 
             if (firstTokenOfTheLine) {
                firstTokenOfTheLine = false;
            } else {
                return ERR_SYN;
            }   

            rett = keyWords();
            if (rett != OK)
                return rett;

            break;
        case sIdentificator:
        //ak neni prvy na riadku tak je to chyba 
            if (firstTokenOfTheLine) {
                firstTokenOfTheLine = false;
            } else {
                return ERR_SYN;
            }
                
            if ((rett = declaration()) != OK) {
                return rett;
            }

            break;
        case sEOL:
            firstTokenOfTheLine=true;
            break;
        case sDef:
            //ak neni prvy na riadku tak je to chyba 
            if (firstTokenOfTheLine) {
                firstTokenOfTheLine = false;
            } else {
                return ERR_SYN;
            }                

            firstTokenOfTheLine = false;
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
        default: 
            return ERR_SYN;
    }
    }
       
    


}

// fce na rozdeleni keywordu
int keyWords() {
    int rett= OK;

    switch (aktToken->type) {
        case sPrint:
            aktToken = get_token();

            if(aktToken->type == sLexError) {
                return ERR_LEX;
            }
                
            if (aktToken->subtype != sLeftBracket) {
                return ERR_SYN;
            }
                
           /* ??????????????????????????????????????????
            operand1 = initOperand(operand1, "", sIdentificator, true, false, false, "GF");
            instruction1op(&instList, WRITE, operand1);*/
            prevToken = aktToken;
            //printf("\npriiiint\n");
            while((aktToken = get_token())) {
                if (aktToken->type == sLexError) {
                    return ERR_LEX;
                }
                    
                switch (aktToken->type) {
                    case sString:
                        operand1 = initOperand(operand1,aktToken->data.value,false,false,aktToken->type,aktToken->subtype,"GF");
                        instruction1op(&instList, WRITE, operand1);
                        break;

                    case sComma:
                        if(prevToken->subtype == sLeftBracket) {
                            return ERR_SYN;
                        }                           
                        break;

                    case sIdentificator:
                        if(prevToken->type != sComma) {
                            return ERR_SYN;
                        }                           
                        rett = checkVariable();
                        if(rett != OK) {
                            return rett;
                        }                            
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
            aktToken = get_token();
            if(aktToken->type == sLexError) {
                return ERR_LEX;
            }

            rett=exprParsing(aktToken);
            if (rett != OK) {
                return rett;
            }
            aktToken=get_token();
            if(aktToken->type == sLexError) {
                return ERR_LEX;
            }

            if (aktToken->type != sEOL)
                return ERR_SYN;
            

            aktToken=get_token();
            if(aktToken->type == sLexError) {
                return ERR_LEX;
            }

            if (aktToken->type != sIndent)
                return ERR_SYN;
            //****************zacina telo ifu************************
            rett = doParse() ;
            if(rett  != sDedent) {
                return ERR_SYN;
            }

            /***********************nasleduje else************************ */
            aktToken = get_token();
            if(aktToken->type == sLexError) {
                return ERR_LEX;
            }

            if (aktToken->type != sElse)
                return ERR_SYN;
            

            aktToken=get_token();
            if(aktToken->type == sLexError) {
                return ERR_LEX;
            }
            if (aktToken->type != sEOL)
                return ERR_SYN;

            aktToken=get_token();
            if(aktToken->type == sLexError) {
                return ERR_LEX;
            }
            if (aktToken->type != sIndent)
                return ERR_SYN;

            rett = doParse() ;
            if(rett  != sDedent) {
                return ERR_SYN;
            }

            return OK;
        case sWhile:
            //call expression
            return OK;
        case sReturn:
            if (inMain) {
                return ERR_SEM_FCE;
            }
                
            // call instruction gen return with parametr
            instruction0op(&instList,RETURN);
            return OK;
    }
    return OK;
}

// zjištění jeslti se jedná o proměnnou nebo o funkci nebo argument
int declaration() {
    int rett= OK;
    if (argumentBool == true) { // kdyz zpracovavam argumenty tak se uz muzu vratit
        prevToken = aktToken;
        /*??????????????????????
        rett = declarationVariable();
        return rett;*/
        return OK;
    }

    prevToken = aktToken;
    aktToken = get_token();
    if(aktToken->type == sLexError) {
        return ERR_LEX;
    }
        
    //printf("Hodnota prev tokenu: %s, Typ tokenu: %d\n", prevToken->data.value, prevToken->type);
    //printf("Hodnota akt tokenu: %s, Typ tokenu: %d\n", aktToken->data.value, aktToken->type);
    if (aktToken->type == sAssignment)
    {
        rett = declarationVariable();
        return rett;
    }
    
            
    if (aktToken->subtype == sLeftBracket){
        rett = checkFunction();
            return rett;
    }
            
    
    return ERR_SYN;
}

// deklarace proměnné
int declarationVariable() {
    int rett= OK;
   
    if (inMain) { // v mainu
       if ((symTableSearch(&gTable, prevToken->data)) == NULL) {   
            symTableInsertVariable(&gTable, prevToken->data);
            operand1 = initOperand(operand1,prevToken->data.value,false,false,sIdentificator,prevToken->subtype,"GF");
            instruction1op(&instList,DEFVAR,operand1);
        }
        
        node = symTableSearch(&gTable, prevToken->data);
    } else { // ve funkci
        if ((symTableSearch(&lTable, prevToken->data)) == NULL) {   
            symTableInsertVariable(&lTable, prevToken->data);
            operand1 = initOperand(operand1,prevToken->data.value,false,false,sIdentificator,prevToken->subtype,"LF");
            instruction1op(&instList,DEFVAR,operand1);
        }
        
        node = symTableSearch(&lTable, prevToken->data);

    }



    aktToken=get_token();
     if(aktToken->type == sLexError) {
         return ERR_LEX;
     }
        
    if (aktToken->type == sIdentificator) {    
            tToken* temp;
            temp = get_token();

            if(temp->type == sLexError) {
                return ERR_LEX;
            }
                
            if (temp->subtype == sLeftBracket) {
               /* SKONTROLUJ PARAMETRE FUNKCIE*/
                
               rett =checkFunctionParams(aktToken,((tFunction *) node->Data)->paramCounter);
            } else {
                unget_token(temp);
            }            

            rett = exprParsing(aktToken);
            if(rett != OK)
                return rett;
    } else {
        rett = exprParsing(aktToken);
        if(rett != OK) {
            return rett;
        }           
    }
    
    //printf("%d\n", rett);
    //printf("%s\n", node->Key);

    if(inMain) {
            operand1 = initOperand(operand1,prevToken->data.value,false,false,prevToken->type,prevToken->subtype,"GF");
            operand2 = initOperand(operand2, "tmp",false, true,sIdentificator,-1,"GF" );
    } else {
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
            if (node == NULL) {
                return ERR_SEM_FCE;
            }                
        } else {
            node = symTableSearch(&lTable, aktToken->data);
            if (node == NULL) {
                return ERR_SEM_FCE;
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

// kontrola, jeslti bylla uz funkce definovana
int checkFunction() {
    int rett= OK;
    if (symTableSearch(&gTable, prevToken->data) == NULL) {
        symTableInsertFunction(&gTable, prevToken->data);
    } else {
        node = symTableSearch(&gTable, prevToken->data);
    }
    rett =doParse();
            if (rett != OK)
                return rett;
    /*while () {
        if (aktToken->type != sIdentificator)
            return ERR_SEM_FCE;
        // dodelat kontroli jestli to bylo uz definovane nebo jeslti se to ma pridat a pak smazat
    }*/

    return OK;
}

// deklarace funkce
int declarationFunctionHead() {
    int rett= OK;
    aktToken = get_token();
    if (aktToken->type == sLexError){
        return ERR_LEX;}    
    if (aktToken->type != sIdentificator){
        return ERR_SYN;
    }
        
    functionName = aktToken->data;
    //kontrola, jestli už nebyla definovana
    if (symTableSearch(&gTable, aktToken->data) != NULL) {
        return ERR_SEM_VAR;
    } else {
        symTableInsertFunction(&gTable, aktToken->data);
        node = symTableSearch(&gTable, aktToken->data);
        operand1 = initOperand(operand1,aktToken->data.value,true,false,aktToken->type,aktToken->subtype,"GF");
        //instruction1op(&instList,LABEL,operand1);
    }
    /*????? toto tu preco volas?
    rett =doParse();
    if (rett != OK)
       return rett;
    */
   
   prevToken = aktToken;
   aktToken = get_token();
   if(aktToken->type == sLexError){
       return ERR_LEX;
   }
        

    if (aktToken->subtype != sLeftBracket){
        return ERR_SYN;
    }
        
    rett = loadParams(functionName);
   //??????? tej funkcii nechapem ale ze vobec naco tam je a jak funguje
   // rett = nextParametr();

    rett = declarationFunctionBody();
    return rett;
}

int declarationFunctionBody() {
    inMain = false;
    //ERR_VAL rett;
    
    symTableInit(&lTable);
    gNode = symTableSearch(&gTable, functionName);
    int ret = OK;
    aktToken = get_token();
    if(aktToken->type == sLexError)
        return ERR_LEX;

    if (aktToken->type != sEOL)
        return ERR_SYN;

    aktToken = get_token();
    if(aktToken->type == sLexError)
        return ERR_LEX;

    if (aktToken->type != sIndent)
        return ERR_SYN;

    ret = doParse();
    if (ret != sDedent)
        return ret;

    inMain = true;
    return ret;
}


int loadParams()
{
    
    bool ciarka = false;//ID musi mat ciarku pred sebou
    bool isFirst = true;// ak je prvy nemusi mat ciarku pred sebou
    tBSTNodePtr IDFunkcie = symTableSearch(&gTable,functionName);
   
   while (1)
   {
       aktToken = get_token();
        if(aktToken->type == sLexError)
            return ERR_LEX;


        if (aktToken->type == sIdentificator)
        {
            if (isFirst)
            {
                isFirst=false;
                ciarka = false;
                //zvys pocet parametrov do symtable
                ((tFunction *) IDFunkcie->Data)->paramName[((tFunction *) IDFunkcie->Data)->paramCounter] = aktToken->data ;
                (((tFunction *) IDFunkcie->Data)->paramCounter)++ ;
            }
            else 
            {
                if (ciarka)
                {
                       //zvys pocet parametrov
                    ((tFunction *) IDFunkcie->Data)->paramName[((tFunction *) IDFunkcie->Data)->paramCounter] = aktToken->data ;
                    (((tFunction *) IDFunkcie->Data)->paramCounter)++ ;
                    ciarka = false;
                }
                else 
                {
                    return ERR_SYN;
                }
            }
        }     
        else if (aktToken->type == sComma) {
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
            }
        else {
            return ERR_SYN;
        }
        }            
    } 
