#include "functionParser.h"

extern tSymtable gTable;            //GL tabulka symbolů
extern tSymtable lTable;            //Lokální tabulka
extern tDLListInst instList;        //instruction list
extern tDLListInst funcList;        //function list
extern bool inFunction;             //ak true zapisuje sa do funcList
bool global;                 //true ak je premenna v GF

int checkIfExist(tToken*);


int checkFunctionParams(tToken* ID, int numberOfParam )
{
    tToken* aktToken;

    if (checkIfExist(ID) !=OK){
        return ERR_SEM_FCE;
    }


    aktToken = get_token();
    if (aktToken->type == sLexError ){
            return ERR_LEX;
    }

    if(aktToken->type == sIdentificator && numberOfParam>0){
            if (checkIfExist(aktToken) !=OK)
                return ERR_SEM_VAR;
        
        }
    else if ((aktToken->type == sNumber || aktToken->type == sString) && numberOfParam > 0 )
        {
            ;
        }
    else if (aktToken->subtype == sRightBracket)
        return OK;
    else 
        return ERR_SEM_FCE;

    tOperand operand1;
    if (global){
            operand1 = initOperand(operand1,aktToken->data.value,false,false,aktToken->type,aktToken->subtype,"GF");
        }else {
            operand1 = initOperand(operand1,aktToken->data.value,false,false,aktToken->type,aktToken->subtype,"LF");
        }  
    
    if (inFunction){
        instruction1op(&funcList,PUSHS,operand1); 
    } else{
        instruction1op(&instList,PUSHS,operand1); 
    }

    aktToken=get_token();
    if (aktToken->type == sLexError ){
            return ERR_LEX;
    }
    if (aktToken->subtype == sRightBracket)
        return OK;
    else 
        unget_token(aktToken);
        
    for (int i = 1;i < numberOfParam;i++)
    {
        //Kontrola ci je pred dalsim parametrom ciarka
        aktToken = get_token();
        if (aktToken->type == sLexError )
            return ERR_LEX;
        if (aktToken->type != sComma)
            return ERR_SYN;
        

        aktToken=get_token();
        if (aktToken->type == sLexError )
            return ERR_LEX;

        if(aktToken->type == sIdentificator)
        {
            if (checkIfExist(aktToken) !=OK)
                return ERR_SEM_VAR;
        }
        else if (aktToken->type == sNumber || aktToken->type == sString )
        {
            ;
        }
        else 
            return ERR_SEM_FCE;

        /*PUSH TOKEN NA STACK ALEBO NIECO TAKE*/
        if (global){
            operand1 = initOperand(operand1,aktToken->data.value,false,false,aktToken->type,aktToken->subtype,"GF");
        }else {
             operand1 = initOperand(operand1,aktToken->data.value,false,false,aktToken->type,aktToken->subtype,"LF");
        }    
        
        
        if (inFunction){
            instruction1op(&funcList,PUSHS,operand1); 
        } else
        {
            instruction1op(&instList,PUSHS,operand1); 
        }     

    }
    aktToken=get_token();
    if (aktToken->type == sLexError ){
            return ERR_LEX;
    }
    if (aktToken->subtype != sRightBracket)
        return ERR_SYN;
    return OK;
}

int checkIfExist(tToken* token)
{
    if (symTableSearch(&gTable, token->data) != NULL){
        global=true;
        return OK;
    }
    if  (symTableSearch(&lTable, token->data) != NULL){
        global=false;
        return OK;
    }else
        return ERR_SEM_VAR;
        
    
    return OK;
}