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


exprTable Table[15][15] =
{		//+   	-    	*   	/   	\  		=     	<    	>   	<=   	>=   	<>   	(    	)   	EOL   	VAR
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

int tableIndexSelect(tRedukToken token){

    if (token.Token->type == NULL)
    return 13;

    if (token.Token->type == sIdentificator || token.Token->type == sIdentificator)
        return 14;
    
    int arr[14] = {sPlus,sMinus,sMultiplication,sDivideInteger,sDivideFloat,sEqual,sLess,sMore,sMoreEqual,sLessEqual,
				sInequal,sLeftBracket,sRightBracket,sEOL};

        for (int i = 0; i < 14 ; i++ )
         {
             if (token.Token->type == arr[i])
                return i;
         }   
}



ERR_VAL *exprParing(tToken *dostanyToken)
{
    ptrStack stack;
    ptrStack Rstack;
    SInit (&stack);
    SInit (&Rstack);
    tRedukToken t;
    t.Token = (tToken*) malloc(sizeof(int)*16);
    t.Token->type = sEOL;
    stringAddString((&t.Token->data),"EOL");
    SPush(&stack,&t);
    tRedukToken actToken;

    bool redukovane= false;

    if (dostanyToken == NULL)
    {
       actToken.Token = get_token();
    }
    else 
    {
        actToken.Token = dostanyToken;
    }

    if (actToken.Token->type ==sEOL || (actToken.Token->type >=7 && actToken.Token->type <=35) || actToken.Token->type == 38)
        ///VYHOD CHYBU
        return ERR_SYN;

    do
    {
        int indexInTable;

        if(redukovane == false)
			indexInTable = tableIndexSelect(stackTop(&stack));
		else
		{
			tRedukToken temp = *(stackTop(&stack));
			stackPop(&stack);
			indexInTable = tableIndexSelect(stackTop(&stack));
			stackPush(&stack,temp);
		}

        switch (Table[indexInTable][selectIndexTable(actToken)])
        {
        case H:
            {
                int pocetOperatorov = 0;
                while(1)
                {
                    if (stackTop(&stack)->priority == L)
                    {
                        stackPop(&stack);
                        break;
                    }
                    if (stackTop(&stack)->token->type == sOperand)
                    {
                        pocetOperatorov++;
                        stackPush(&Rstack,*(stackTop(&stack)));
                        stackPop(&stack);
                    }
                    if (stackTop(&stack)->token->type == sEOL)
                    break;
                }
                if (pocetOperatorov > 1 )
                {
                    return ERR_SYN;
                }
                zjednodus(&stack,&Rstack,&redukovane);
            }
            break;
        case L:



        default:
            return ERR_SYN;
            
        }


    } while(!( actToken.Token->type == sEOL && stackTop(&stack)->Token->type == sEOL));
    
}