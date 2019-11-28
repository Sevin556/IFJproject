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
#include "exprParser.h"
#include "scanner.h"
#include "stack.h"
#include "string.h"
#include "lifo.h"

extern tSymtable glSymTable;            //GL tabulka symbolů
extern tSymtable LokalTable;                 //Lokální tabulka
extern bool inFunctionBody;             //Indikátor, že se kontroluje tělo funkce
ERR_VAL ret;
exprTable Table[15][15] =
{		//+   	-    	*   	/   	\  		=     	<    	>   	<=   	>=   	<>   	(    	)   	EOL/:   	VAR
/*"+"*/	{H,		H,		L,		L,		L,		H,		H,		H,		H,		H,		H,		L,		H,		H,		L},
/*"-"*/	{H,		H,		L,		L,		L,		H,		H,		H,		H,		H,		H,		L,		H,		H,		L},
/*"*"*/	{H,		H,		H,		H,		H,		H,		H,		H,		H,		H,		H,		L,		H,		H,		L},
/*"/"*/	{H,		H,		H,		H,		H,		H,		H,		H,		H,		H,		H,		L,		H,		H,		L},
/*"\"*/	{H,		H,		H,		H,		H,		H,		H,		H,		H,		H,		H,		L,		H,		H,		L},
/*"="*/	{L,		L,		L,		L,		L,		H,		L,		L,		L,		L,		H,		L,		H,		H,		L},
/*"<"*/	{L,		L,		L,		L,		L,		H,		H,		H,		H,		H,		H,		L,		H,		H,		L},
/*">"*/	{L,		L,		L,		L,		L,		H,		H,		H,		H,		H,		H,		L,		H,		H,		L},
/*"<="*/{L,		L,		L,		L,		L,		H,		H,		H,		H,		H,		H,		L,		H,		H,		L},
/*">="*/{L,		L,		L,		L,		L,		H,		H,		H,		H,		H,		H,		L,		H,		H,		L},
/*"<>"*/{L,		L,		L,		L,		L,		H,		L,		L,		L,		L,		H,		L,		H,		H,		L},
/*"("*/	{L,		L,		L,		L,		L,		L,		L,		L,		L,		L,		L,		L,		EQ,		ERROR,	L},
/*")"*/	{H,		H,		H,		H,		H,		H,		H,		H,		H,		H,		H,		ERROR,	H,		H,		ERROR},
/*"EOL"*/{L,	L,		L,		L,		L,		L,		L,		L,		L,		L,		L,		L,		ERROR,	EXITPARSE,	L},
/*"VAR"*/{H,	H,		H,		H,		H,		H,		H,		H,		H,		H,		H,		ERROR,	H,		H,		ERROR}
};

int tableSelect(tRedukToken* token){

    if (token->type == NULL)
    return 13;

    if (token->type == sIdentificator || token->type == sIdentificator)
        return 14;
    
    int arr[14] = {sPlus,sMinus,sMultiplication,sDivideInteger,sDivideFloat,sEqual,sLess,sMore,sMoreEqual,sLessEqual,
				sInequal,sLeftBracket,sRightBracket,sEOL};

        for (int i = 0; i < 14 ; i++ )
         {
             if (token->type == arr[i])
                return i;
         }   
}



ERR_VAL *exprParing(tToken *dostanyToken)
{
    ptrStack exprStack;
    ptrStack Rstack;
    SInit (&exprStack);
    SInit (&Rstack);
    tRedukToken* firstToken;
    firstToken = (tRedukToken*) malloc(sizeof(int)*32);
    firstToken->TokenType = sDollar;
    firstToken->type = sDollar;
    stringAddString((&firstToken->tokenData),"Dollar");
    SPush(&exprStack,&firstToken);
    tToken *actToken;

    if (dostanyToken == NULL)
    {
       actToken = get_token();
    }
    else 
    {
        actToken = dostanyToken;
    }

    int indexInTable = tableIndexSelect(stackTop(&exprStack));
    while (1)
    {
       switch (Table[indexInTable][selectIndexTable(actToken)])
        {
        case H:
            ret = shiftToStack(&exprStack,actToken);
            if (ret != OK)
            return ret;
            actToken = get_token();
            break;
        case L:
            ret = ApplyRule(&exprStack);
            if (ret != OK)
            return ret;
            break;
        case EQ:

        }
    }

}



tRedukToken* createNewToken(tToken *token)
{
    tRedukToken *temp;
    temp = (tRedukToken*) malloc(sizeof(int)*32);
    if (temp == NULL)
    temp->Redukovany=false;
    temp->TableIndex=selectIndexTable(token);
    temp->tokenData=token->data;
    temp->TokenType=token->type;
    temp->subtype= token->subtype;
    return &(temp);
}


ERR_VAL shiftToStack (ptrStack *stack,tRedukToken* token)
{
    if (!stackEmpty(&stack))
    {
        stackTop(&stack)->handle= true;
            
        tRedukToken* new_token = createNewToken(&token);

        if (new_token->TokenType  == sIdentificator)
        {
                tBSTNodePtr ID_uzlu = symTableSearch(&glSymTable,(*new_token->tokenData));

            if ( ID_uzlu != NULL)
            {
                if(inFunctionBody)
                {
                    ID_uzlu = symTableSearch(&LokalTable,(*new_token->tokenData));
                    if ( ID_uzlu == NULL)
                    {
                        return ERR_SEM_VAR;
                    }
                }
                else 
                return ERR_SEM_VAR;
            }
            else  //ak sme ju nasli v globalnej a sme aj 
            {
                if(inFunctionBody)
                {
                    ID_uzlu = symTableSearch(&LokalTable,(*new_token->tokenData));
                    if ( ID_uzlu == NULL)
                    {
                        return ERR_SEM_VAR;
                    }
                }
                else 
                return ERR_SEM_VAR;
            }


        }

        stackPush (&stack,new_token);
        return OK;
    }
}


ERR_VAL ApplyRule();    