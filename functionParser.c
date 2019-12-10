#include "functionParser.h"

extern tSymtable gTable;            //GL tabulka symbolů
extern tSymtable lTable;            //Lokální tabulka
extern tDLListInst instList; 


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

    if(aktToken->type == sIdentificator && numberOfParam>0)
        {
            if (checkIfExist(aktToken) !=OK)
                return ERR_SEM_VAR;
        }
        else if ((aktToken->type == sNumber || aktToken->type == sString) && numberOfParam > 0 )
        {
            ;
        }
        else 
            return ERR_SEM_FCE;
                   
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
        
        
        
        

    }
    return OK;
}

int checkIfExist(tToken* token)
{
    if (symTableSearch(&gTable, token->data) == NULL)
        return ERR_SEM_VAR;
   
    return OK;
}