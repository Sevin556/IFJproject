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
ERR_VAL ApplyRule(tStack*);
ERR_VAL shiftToStack (tStack*,tToken*);
tRedukToken* createNewToken(tToken*);
ERR_VAL checkOperator(tStack* ,int);
ERR_VAL checkSemantic(tRedukToken* ,tRedukToken* , int );
ERR_VAL reduceBrackets(tStack*);

extern tSymtable glSymTable;            //GL tabulka symbolů
extern tSymtable LokalTable;                 //Lokální tabulka
extern bool inFunctionBody;             //Indikátor, že se kontroluje tělo funkce
ERR_VAL ret;                            //return value
int IndexTerminalu = -1;                     //index terminalu v stacku 


exprTable Table[15][15] =
{		//+   	-    	*   	/   	//  	=     	<    	>   	<=   	>=   	<>   	(    	)   	EOL/:   VAR
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
/*"$  "*/{L,	L,		L,		L,		L,		L,		L,		L,		L,		L,		L,		L,		ERROR,	EXITPARSE,	L},
/*"VAR"*/{H,	H,		H,		H,		H,		H,		H,		H,		H,		H,		H,		ERROR,	H,		H,		ERROR}
};
/**
 * @brief vrati index zadaneho operatora v tabulke
 */
int tableSelect(tRedukToken* token){

    if (token->type == NULL)
    return 13;

    if (token->TokenType == sIdentificator || token->TokenType == sNumber || token->TokenType == sString)
        return 14;
    
    int arr[15] = {sPlus,sMinus,sMultiplication,sDivideInteger,sDivideFloat,sEqual,sLess,sMore,sMoreEqual,sLessEqual,
				sInequal,sLeftBracket,sRightBracket,sEOL,sColon};

        for (int i = 0; i < 15 ; i++ )
         {
             if (token->TokenType == arr[i])
                if (i==14)
                {
                    i =13;
                }
                
                return i;
         }   
}


/**
 * @brief volana parserom na skontrolovanie vyrazu
 */
ERR_VAL *exprParing(tToken *dostanyToken)
{
    tStack exprStack;
    tStack Rstack;
    SInit (&exprStack);
    SInit (&Rstack);
    tRedukToken* firstToken;
    firstToken = (tRedukToken*) malloc(sizeof(int)*32);
    firstToken->TokenType = sDollar;
    firstToken->type = sDollar;
    stringAddString((&firstToken->tokenData),"Dollar");
    shiftToStack(&exprStack,firstToken);
    IndexTerminalu = 0;
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
        case EQ://len pri redukovani zatvoriek
            shiftToStack(&exprStack,actToken);

             if ((ret = reduceBrackets(&exprStack)) != OK) {
                    return ret;
                }
            actToken = get_token();
        case ERROR: 
                return ERR_SYN;
        case EXITPARSE:
            return OK;
        }
    }

}

/**
 * @brief vytvori novy token do stacku
 */
tRedukToken* createNewToken(tToken *token)
{
    tRedukToken *temp;
    temp = (tRedukToken*) malloc(sizeof(int)*32);
    if (temp == NULL)
    temp->Redukuj=false;
    temp->terminal = true;
    temp->TableIndex=selectIndexTable(token);
    temp->tokenData=token->data;
    temp->TokenType=token->type;
    temp->subtype= token->subtype;
    return &(temp);
}
/**
 * @brief pushne token na stack a skontroluje pri ID ci existuje
 */
ERR_VAL shiftToStack (tStack *stack,tToken* token)
{
    if (!stackEmpty(&stack))
    {
        stackTop(&stack)->Redukuj= true;
            
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
                    else 
                    {
                        /*ULOZ JEJ TYP*/
                       // new_token->type = ((tDataFunction *) node->Data)->declared = true;
                    }
                }
                else 
                return ERR_SEM_VAR;
            }
            else  //ak sme ju nasli v globalnej a musi byt aj v lokalnej
            {
                if(inFunctionBody)
                {
                    ID_uzlu = symTableSearch(&LokalTable,(*new_token->tokenData));
                    if ( ID_uzlu == NULL)
                    {
                        return ERR_SEM_VAR;
                    }
                    else 
                    {
                        /*ULOZ JEJ TYP*/
                       // new_token->type = ((tDataFunction *) node->Data)->declared = true;
                    }
                }
                else 
                return ERR_SEM_VAR;
            }


        }

        stackPush (&stack,new_token);
        IndexTerminalu = IndexOfTop(&stack);
        return OK;
    }
}
/**
 * @brief pouzije pravidlo
 */

ERR_VAL ApplyRule(tStack* stack)
{
    ERR_VAL ret = OK;
    if(!stackEmpty(&stack))
    {
            tRedukToken* temp =stackIndex(&stack,IndexTerminalu);

        switch(temp->TokenType)
        {   //situacia pre operandy
            case sIdentificator:   
                                temp->terminal = false;
                                temp->Redukuj = false;
                                tRedukToken* temp2 =stackTopPop(&stack);
                                stackTop(&stack)->Redukuj = false;
                                stackPush(&stack,temp2);
                                /*generuj KOD pre tlacenie*/
                                break;
            case sOperand:
                            switch(temp->subtype)
                                {
                                    case sPlus:
                                            if (ret = checkOperator(&stack,temp->subtype) != OK)
                                            return ret;

                                            /*JE TO ZLOZITEJSIE DOPLN KOD 
                                            vygeneruj kod */
                                            break;
                                    case sMultiplication:
                                            if (ret = checkOperator(&stack,temp->subtype) != OK)
                                            return ret;

                                            /*vygeneruj kod */
                                            break;
                                    case sDivideFloat:
                                            if (ret = checkOperator(&stack,temp->subtype) != OK)
                                            return ret;

                                            /*vygeneruj kod */
                                            break;
                                    case sDivideInteger:
                                            if (ret = checkOperator(&stack,temp->subtype) != OK)
                                            return ret;

                                            /*vygeneruj kod */
                                            break;
                                    case sMinus:
                                            if (ret = checkOperator(&stack,temp->subtype) != OK)
                                            return ret;

                                            /*vygeneruj kod */
                                            break;
                                    case sEqual:
                                            if (ret = checkOperator(&stack,temp->subtype) != OK)
                                            return ret;

                                            /*vygeneruj kod */
                                            break;
                                    case sInequal:
                                            if (ret = checkOperator(&stack,temp->subtype) != OK)
                                            return ret;

                                            /*vygeneruj kod */
                                            break;
                                    case sMore:
                                            if (ret = checkOperator(&stack,temp->subtype) != OK)
                                            return ret;

                                            /*vygeneruj kod */
                                            break;
                                    case sMoreEqual:
                                            if (ret = checkOperator(&stack,temp->subtype) != OK)
                                            return ret;

                                            /*vygeneruj kod */
                                            break;
                                    case sLess:
                                            if (ret = checkOperator(&stack,temp->subtype) != OK)
                                            return ret;

                                            /*vygeneruj kod */
                                            break;
                                    case sLessEqual:
                                            if (ret = checkOperator(&stack,temp->subtype) != OK)
                                            return ret;

                                            /*vygeneruj kod */
                                            break;
                                    case sRightBracket:
                                            if (ret = doBracket(&stack,temp->subtype) != OK)
                                            return ret;

                                            /*vygeneruj kod */
                                            break;
                                    default :
                                    return ERR_SYN;
                                }
                            break;
        }
        stackPop(&stack);
        stackPop(&stack);
    }
    else 
    return ERR_SYN;
}  



/**
 * @brief skontroluje platnost operacie
 */
ERR_VAL  checkOperator(tStack* stack,int znamienko)
{
    if (!stackEmpty(&stack))
    {
        tRedukToken* RightOperand= stackTop(&stack);
        if (RightOperand->TableIndex != 14)
        return ERR_SYN;

        stackPop(&stack);

        tRedukToken* operator = stackTop(&stack);
        stackPop(&stack);
        tRedukToken* LeftOperand = stackTop(&stack);
        if (LeftOperand->TableIndex != 14 || LeftOperand->terminal !=false ||
            operator->terminal != true || operator->TokenType != sOperand || 
            RightOperand->terminal !=false || RightOperand->TableIndex !=14)
        {
            return ERR_SYN;
        }

        if (checkSemantic(LeftOperand,RightOperand,znamienko) !=OK )
        {
            return ERR_RUN;
        }
        
        stackPush(&stack,LeftOperand);
        stackPush(&stack,operator);
        stackPush(&stack,RightOperand);
        return OK;
    }
    else
    return ERR_SYN;
}

/**
 * @brief skontroluje ci su typy rovnake alebo ich pretypuje
 */
ERR_VAL checkSemantic(tRedukToken* LeftOperand,tRedukToken* RightOperand, int operation)
{
    switch (operation)
    {
        case sPlus: 
                if (LeftOperand->type ==sInteger && RightOperand == sInteger)
                {
                    return OK;
                }
                else if (LeftOperand->type == sDoublePoint && RightOperand == sInteger)
                {
                    RightOperand->type = sDoublePoint;
                    /*PRETYPUJ*/
                }
                else if (LeftOperand->type ==sInteger && RightOperand ==  sDoublePoint)
                {
                    LeftOperand->type = sDoublePoint;
                    /*PRETYPUJ*/
                }
                else if (LeftOperand->type ==sDoublePoint && RightOperand ==  sDoublePoint)
                {
                    return OK;
                }
                else if (LeftOperand->type ==sString && RightOperand ==  sString)
                {
                    return OK;
                }
                else 
                return ERR_RUN;
                break;

        case sDivideFloat:
                if (LeftOperand->type ==sInteger && RightOperand == sInteger)
                {   
                    LeftOperand->type = sDoublePoint;
                    RightOperand->type = sDoublePoint;
                   /*PRETYPUJ OBA*/
                }
                else if (LeftOperand->type == sDoublePoint && RightOperand == sInteger)
                {
                    RightOperand->type = sDoublePoint;
                    /*PRETYPUJ*/
                }
                else if (LeftOperand->type ==sInteger && RightOperand ==  sDoublePoint)
                {
                    LeftOperand->type = sDoublePoint;
                    /*PRETYPUJ*/
                }
                else if (LeftOperand->type ==sDoublePoint && RightOperand ==  sDoublePoint)
                {
                    return OK;
                }
                else 
                {
                    return ERR_RUN;
                }
                break;
         case sDivideInteger:
                if (LeftOperand->type ==sInteger && RightOperand == sInteger)
                {
                   return OK;
                }
                else 
                {
                    return ERR_RUN;
                }
        case sMultiplication:
        case sMinus:
                if (LeftOperand->type ==sInteger && RightOperand == sInteger)
                {
                    return OK;
                }
                else if (LeftOperand->type == sDoublePoint && RightOperand == sInteger)
                {
                    RightOperand->type = sDoublePoint;
                    /*PRETYPUJ*/
                }
                else if (LeftOperand->type ==sInteger && RightOperand ==  sDoublePoint)
                {
                    LeftOperand->type = sDoublePoint;
                    /*PRETYPUJ*/
                }
                else if (LeftOperand->type ==sDoublePoint && RightOperand ==  sDoublePoint)
                {
                    return OK;
                }
                else 
                {
                    return ERR_RUN;
                }
                break;
        case sEqual:
        case sLessEqual:
        case sMoreEqual:
        case sLess:
        case sMore:
        case sInequal:
                break;
    }
}

ERR_VAL reduceBrackets(tStack *stack)
{
    if (!stackEmpty(&stack))
    {
        tRedukToken* temp1 = stackTopPop(&stack);
        tRedukToken* temp2 = stackTopPop(&stack);
        tRedukToken* temp3 = stackTopPop(&stack);

        if (temp2->Redukuj == false && temp3->subtype == sLeftBracket)
        {
            
            if (stackTop(&stack)->terminal == true)
            {
                IndexTerminalu = IndexOfTop(&stack);
                stackIndex(&stack,IndexTerminalu)->handle = false;
            }
            stackPush(&stack,temp2);
        }
        else 
        {
            stackPush(&stack,temp3);
            stackPush(&stack,temp2);
            stackPush(&stack,temp1);
            return ERR_SYN;
        }
    }
    else 
        return ERR_SYN;

    return OK;
     
}