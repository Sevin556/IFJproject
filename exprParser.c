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
#include "exprParser.h"


 tSymtable gTable;            //GL tabulka symbolů
 tSymtable lTable;            //Lokální tabulka
extern bool inMain;         //Indikátor, že se kontroluje tělo funkce
extern tDLListInst instList; 
extern tBSTNodePtr *node;             
//bool inFunctionBody;             
int ret;                            //return value
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
/*"("*/	{L,		L,		L,		L,		L,		L,		L,		L,		L,		L,		L,		L,		EQUAL,		H,	    L},
/*")"*/	{H,		H,		H,		H,		H,		H,		H,		H,		H,		H,		H,		ERROR,	H,		H,	ERROR},
/*"$  "*/{L,	L,		L,		L,		L,		L,		L,		L,		L,		L,		L,		L,		ERROR,	EXITPARSE,	    L},
/*"VAR"*/{H,	H,		H,		H,		H,		H,		H,		H,		H,		H,		H,		ERROR,	H,		H,		ERROR}
};

/**
 * @brief vrati index zadaneho operatora v tabulke
 */
int tableIndexSelect(int type, int subtype){

    //if (token->TokenType == NULL)
    //return 13;
    //printf("EXPR INDEX zacinam s %d a %d\n ",type,subtype);
    if (type == sDollar)
    {
        return 13;
    }
    if (type == sIdentificator || type == sNumber || type == sString)
    {
        printf("EXPR INDEXTABLE vybral som 14 \n");
        return 14;
    }    
    
    int arr[15] = {sPlus,sMinus,sMultiplication,sDivideInteger,sDivideFloat,sEqual,sLess,sMore,sMoreEqual,sLessEqual,
				sInequal,sLeftBracket,sRightBracket,sEOL,sColon};
    //printf("EXPR INDEXTABLE nevybral som  14 \n");
        for (int i = 0; i < 15 ; i++ )
         {
             //printf(" %d a je to porovbnanie %d a %d \n",i,subtype,arr[i]);
             if (subtype == arr[i] || type == arr[i]) {
                // printf("EXPR INDEXTABLE vybral som %d \n",i);
                if (i==14)
                {
                    i =13;
                }
                //printf("EXPR INDEXTABLE vybral som %d \n",i);
                return i;
             }
                
         }   
    return ERROR;
}


/**
 * @brief volana parserom na skontrolovanie vyrazu
 */
int exprParsing(tToken *dostanyToken)
{
    tStack exprStack;
    tStack Rstack;
    stackInit (&exprStack);
    stackInit (&Rstack);
    tRedukToken* firstToken;
    firstToken = (tRedukToken*) malloc(sizeof(int)*32);
    firstToken->TokenType = sDollar;
    firstToken->type = sDollar;
    firstToken->subtype = sDollar;
    firstToken->TableIndex = 13;
    firstToken->terminal = false;
    firstToken->Redukuj = true;
    tOperand operand1 = initOperand(operand1,firstToken->tokenData.value,false,false,firstToken->TokenType,firstToken->subtype,"GF" );
    instruction1op(&instList,PUSHS,operand1);
    
    //stringAddString(firstToken->tokenData,"Dollar");
    stackPush(&exprStack,firstToken);
    IndexTerminalu = 0;
    tToken *actToken;

    if (dostanyToken == NULL)
    {
        
       actToken = get_token();
       printf("EXPR  nacitavam token %s \n",actToken->data.value);
    }
    else 
    {
        actToken = dostanyToken;
        printf("EXPR  mam token %s \n",actToken->data.value);
    }

    //printf("EXPR mam token %s \n",actToken->data.value);
    shiftToStack(&exprStack,actToken);
    actToken = get_token();
    while (1)
    {
        if (actToken->type == sLexError )
        return ERR_LEX;
        
       int indexInTable = tableIndexSelect(stackIndex(&exprStack,IndexTerminalu)->TokenType,stackIndex(&exprStack,IndexTerminalu)->subtype);
       switch (Table[indexInTable][tableIndexSelect(actToken->type,actToken->subtype)])
        {
        case L:
            printf("EXPR ROBIM H s %s \n",actToken->data.value);
            ret = shiftToStack(&exprStack,actToken);
            if (ret != OK)
            return ret;
            actToken = get_token();
             printf("EXPR Koncim H s %s \n",actToken->data.value);
            break;
        case H:
            printf("EXPR ROBIM L s %s\n",actToken->data.value);
            ret = ApplyRule(&exprStack);
            if (ret != OK)
            return ret;
            break;
        case EQUAL://len pri redukovani zatvoriek
            printf("ROBIM EQ \n");
            shiftToStack(&exprStack,actToken);
            ret = reduceBrackets(&exprStack);
             if (ret != OK) {
                    return ret;
                }
            actToken = get_token();
            if (actToken == NULL)
            {
                return ERR_SYN;
            }
            
            break;
        case ERROR:                
            printf("ROBIM ERROR \n");
            return ERR_SYN;
        case EXITPARSE:
            printf(" \n EXPR top stack je %s \n",stackTop(&exprStack)->tokenData.value);
            printf("ROBIM EXIT \n");
            (*node)->nodeDataType = stackTop(&exprStack)->type;
            return OK;
        default:
            printf("robim default");
            return ERR_SYN;
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
    temp->TableIndex=tableIndexSelect(token->type,token->subtype);
    temp->tokenData=token->data;
    temp->TokenType=token->type;
    temp->subtype= token->subtype;
    return temp;
}
/**
 * @brief pushne token na stack a skontroluje pri ID ci existuje
 */
int shiftToStack (tStack *stack,tToken* token)
{
    if (!stackEmpty(stack))
    {
        stackIndex(stack,IndexTerminalu)->Redukuj= true;
            
        tRedukToken* new_token = createNewToken(token);

        if (new_token->TokenType  == sIdentificator)
        {
                tBSTNodePtr ID_uzlu = symTableSearch(&gTable,new_token->tokenData);

            if ( ID_uzlu != NULL)
            {
                if(!inMain)
                {
                    ID_uzlu = symTableSearch(&lTable,new_token->tokenData);
                    if ( ID_uzlu == NULL)
                    {
                        return ERR_SEM_VAR;
                    }
                    else 
                    {
                        /*ULOZ JEJ TYP*/
                     //  new_token->type = ((tDataFunction *) node->Data)->declared = true;
                    }
                }
                else 
                return ERR_SEM_VAR;
            }
            else  //ak sme ju nasli v globalnej a musi byt aj v lokalnej
            {
                if(!inMain)
                {
                    ID_uzlu = symTableSearch(&lTable,new_token->tokenData);
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
        printf("EXPR pushujem %s \n",new_token->tokenData.value);
        stackPush (stack,new_token);
        IndexTerminalu = IndexOfTop(stack); 
    }
    return OK;
}
/**
 * @brief pouzije pravidlo
 */

int ApplyRule(tStack* stack)
{
    int ret = OK;
    if(!stackEmpty(stack))
    {
        tRedukToken* temp =stackIndex(stack,IndexTerminalu);
        printf("EXPR aplikujem pravidlo pre %s \n",temp->tokenData.value);
        switch(temp->TokenType)
        {   //situacia pre operandy
            case sDollar: return OK;
            case sIdentificator:
            case sNumber:
            case sString:
                                 printf("EXPR aplikujem pravidlo sIDEN pre %s \n",temp->tokenData.value);   
                                temp->terminal = false;
                                temp->Redukuj = false;
                                tRedukToken* temp2 =stackTopPop(stack);
                                stackTop(stack)->Redukuj = false;
                                IndexTerminalu = IndexOfTop(stack);
                                stackPush(stack,temp2);
                                tOperand operand1;
                                //generuj KOD pre tlacenie
                                if (inMain)
                                 { 
                                     operand1 = initOperand(operand1, stackTop(stack)->tokenData.value,
                                           false, false,stackTop(stack)->TokenType,stackTop(stack)->subtype,"GF" );
                                          
                                 }
                                else
                                {
                                    operand1 = initOperand(operand1, stackTop(stack)->tokenData.value,
                                           false, false,stackTop(stack)->TokenType,stackTop(stack)->subtype,"LF" );
                                } 
                                instruction1op(&instList, PUSHS, operand1);
                                return OK;
                                break;
            case sOperand:
                             printf("EXPR aplikujem pravidlo sOPERATOR pre %s \n",temp->tokenData.value);
                            switch(temp->subtype)
                                {
                                    case sPlus:
                                           ret=checkOperator(stack,temp->subtype);
                                            if (ret!= OK)
                                            return ret;
                                            /*JE TO ZLOZITEJSIE DOPLN KOD 
                                            vygeneruj kod */
                                            break;
                                    case sMultiplication:
                                            ret=checkOperator(stack,temp->subtype);
                                            if (ret!= OK)
                                            return ret;
                                            instruction0op(&instList,MULS);

                                            /*vygeneruj kod */
                                            break;
                                    case sDivideFloat:
                                            printf("EXPR robim sDivideFloat");
                                            ret=checkOperator(stack,temp->subtype);
                                            if (ret!= OK)
                                            return ret;

                                            instruction0op(&instList,DIVS);
                                            /*vygeneruj kod */
                                            break;
                                    case sDivideInteger:
                                            ret=checkOperator(stack,temp->subtype);
                                            if (ret!= OK)
                                            return ret;
                                            instruction0op(&instList,IDIVS);

                                            /*vygeneruj kod */
                                            break;
                                    case sMinus:
                                            ret=checkOperator(stack,temp->subtype);
                                            if (ret!= OK)
                                            return ret;

                                            /*vygeneruj kod */
                                            instruction0op(&instList,SUBS);
                                            break;
                                    case sEqual:
                                            ret=checkOperator(stack,temp->subtype);
                                            if (ret!= OK)
                                            return ret;

                                            /*vygeneruj kod */
                                            instruction0op(&instList,EQS);
                                            break;
                                    case sInequal:
                                            ret=checkOperator(stack,temp->subtype);
                                            if (ret!= OK)
                                            return ret;

                                            /*vygeneruj kod */
                                            
                                            instruction0op(&instList,EQS);
                                            instruction0op(&instList,NOTS);
                                            break;
                                    case sMore:
                                            ret=checkOperator(stack,temp->subtype);
                                            if (ret!= OK)
                                            return ret;

                                            /*vygeneruj kod */
                                            instruction0op(&instList,GTS);
                                            break;
                                    case sMoreEqual:
                                            ret=checkOperator(stack,temp->subtype);
                                            if (ret!= OK)
                                            return ret;

                                            /*vygeneruj kod */
                                           instruction0op(&instList,GTS);
                                            instruction0op(&instList,NOTS);
                                            break;
                                    case sLess:
                                            ret=checkOperator(stack,temp->subtype);
                                            if (ret!= OK)
                                            return ret;

                                            /*vygeneruj kod */
                                           instruction0op(&instList,LTS);
                                            break;
                                    case sLessEqual:
                                            ret=checkOperator(stack,temp->subtype);
                                            if (ret!= OK)
                                            return ret;
                                            /*vygeneruj kod */
                                           instruction0op(&instList,LTS);
                                           instruction0op(&instList,NOTS);
                                            break;
                                    case sRightBracket:
                                            ret=reduceBrackets(stack);
                                            if (ret!= OK)
                                            return ret;

                                            /*vygeneruj kod */
                                            break;
                                    default :
                                    printf("NEMAM PRAVIDLO");
                                    return ERR_SYN;
                                }
                            break;
        }
        stackPop(stack);
        stackPop(stack);
        IndexTerminalu = IndexOfTop(stack)-1;
        stackIndex(stack,IndexTerminalu)->Redukuj = false;

    }
    else 
        return ERR_SYN;
    return OK;
}  



/**
 * @brief skontroluje platnost operacie
 */
int  checkOperator(tStack* stack,int znamienko)
{
    printf("EXPR zacinam CheckOperator s %d \n",znamienko);
    if (!stackEmpty(stack))
    {
        tRedukToken* RightOperand= stackTopPop(stack);
        printf("EXPR pravy operator je  %s \n",RightOperand->tokenData.value);
        if (RightOperand->TableIndex != 14)
        return ERR_SYN;

        tRedukToken* operator = stackTopPop(stack);
        printf("EXPR  operator je  %s \n",operator->tokenData.value);
        tRedukToken* LeftOperand = stackTopPop(stack);
        if (LeftOperand->TableIndex != 14)
        return ERR_SYN;

        printf("EXPR lavy operator %s \n",LeftOperand->tokenData.value);
        
        
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
        
        stackPush(stack,LeftOperand);
        stackPush(stack,operator);
        stackPush(stack,RightOperand);
        return OK;
    }
    else
    return ERR_SYN;
}

/**
 * @brief skontroluje ci su typy rovnake alebo ich pretypuje
 */
int checkSemantic(tRedukToken* LeftOperand,tRedukToken* RightOperand, int operation)
{
    tOperand operand1;
   tOperand operand2;
    printf("EXPR zacinam semantiku s %s a %s",RightOperand->tokenData.value,LeftOperand->tokenData.value);
    switch (operation)
    {   
        case sEqual:    
        case sLessEqual:
        case sMoreEqual:
        case sLess:
        case sMore:
        case sInequal:
        case sPlus: 
                if (LeftOperand->subtype ==sInteger && RightOperand->subtype == sInteger)
                {
                    return OK;
                }
                else if (LeftOperand->subtype == sDoublePointNumber && RightOperand->subtype == sInteger)
                {
                    RightOperand->type = sDoublePointNumber;
                    /*PRETYPUJ*/
                   if(inMain)
                    {
                         operand1 = initOperand(operand1, RightOperand->tokenData.value,
                                           false, false,RightOperand->TokenType,RightOperand->subtype,"GF" );
                    }
                    else
                    {
                        operand1 = initOperand(operand1, RightOperand->tokenData.value,
                                           false, false,RightOperand->TokenType,RightOperand->subtype,"LF" );
                    }
                    
                    instruction1op(&instList,POPS,operand1);
                    instruction2op(&instList,INT2FLOAT,operand1,operand1);
                    instruction1op(&instList,PUSHS,operand1);


                    return OK;
                }
                else if (LeftOperand->subtype ==sInteger && RightOperand->subtype ==  sDoublePointNumber)
                {
                    LeftOperand->type = sDoublePointNumber;
                    
                    if(inMain)
                    {
                         operand1 = initOperand(operand1, LeftOperand->tokenData.value,
                                           false, false,LeftOperand->TokenType,LeftOperand->subtype,"GF" );
                         operand2 = initOperand(operand2, RightOperand->tokenData.value,
                                           false, false,RightOperand->TokenType,RightOperand->subtype,"GF" );
                    }
                    else
                    {
                        operand1 = initOperand(operand1, LeftOperand->tokenData.value,
                                           false, false,LeftOperand->TokenType,LeftOperand->subtype,"LF" );
                        operand2 = initOperand(operand2, RightOperand->tokenData.value,
                                           false, false,RightOperand->TokenType,RightOperand->subtype,"GF" );
                    }
                    
                    instruction1op(&instList,POPS,operand2);
                    instruction1op(&instList,POPS,operand1);
                    instruction2op(&instList,INT2FLOAT,operand1,operand1);
                    instruction1op(&instList,PUSHS,operand1);
                    instruction1op(&instList,PUSHS,operand2);

                    return OK;
                }
                else if (LeftOperand->subtype ==sDoublePointNumber && RightOperand->subtype ==  sDoublePointNumber)
                {
                    return OK;
                }
                else if (LeftOperand->TokenType ==sString && RightOperand->TokenType ==  sString)
                {
                    return OK;
                }
                else 
                return ERR_RUN;
                break;

        case sDivideFloat:
                if (LeftOperand->subtype ==sInteger && RightOperand->subtype == sInteger)
                {   
                    LeftOperand->type = sDoublePointNumber;
                    RightOperand->type = sDoublePointNumber;
                   /*PRETYPUJ OBA*/
                   if(inMain)
                    {
                         operand1 = initOperand(operand1, RightOperand->tokenData.value,
                                           false, false,RightOperand->TokenType,RightOperand->subtype,"GF" );
                         operand2 = initOperand(operand2, LeftOperand->tokenData.value,
                                           false, false,LeftOperand->TokenType,LeftOperand->subtype,"GF" );
                    }
                    else
                    {
                        operand1 = initOperand(operand1, RightOperand->tokenData.value,
                                           false, false,RightOperand->TokenType,RightOperand->subtype,"LF" );
                        operand2 = initOperand(operand2, LeftOperand->tokenData.value,
                                           false, false,LeftOperand->TokenType,LeftOperand->subtype,"LF" );
                    }
                    
                    instruction1op(&instList,POPS,operand1);
                    instruction2op(&instList,INT2FLOAT,operand1,operand1);
                    instruction1op(&instList,POPS,operand2);
                    instruction2op(&instList,INT2FLOAT,operand2,operand2);
                    instruction1op(&instList,PUSHS,operand2);
                    instruction1op(&instList,PUSHS,operand1);
                    

                    return OK;
                }
                else if (LeftOperand->subtype == sDoublePointNumber && RightOperand->subtype == sInteger)
                {
                    RightOperand->type = sDoublePointNumber;

                    /*PRETYPUJ*/
                    if(inMain)
                    {
                         operand1 = initOperand(operand1, RightOperand->tokenData.value,
                                           false, false,RightOperand->TokenType,RightOperand->subtype,"GF" );
                    }
                    else
                    {
                        operand1 = initOperand(operand1, RightOperand->tokenData.value,
                                           false, false,RightOperand->TokenType,RightOperand->subtype,"LF" );
                    }
                    
                    instruction1op(&instList,POPS,operand1);
                    instruction2op(&instList,INT2FLOAT,operand1,operand1);
                    instruction1op(&instList,PUSHS,operand1);
                    return OK;
                }
                else if (LeftOperand->subtype ==sInteger && RightOperand->subtype ==  sDoublePointNumber)
                {
                    LeftOperand->type = sDoublePointNumber;

                    /*PRETYPUJ*/
                     if(inMain)
                    {
                         operand1 = initOperand(operand1, LeftOperand->tokenData.value,
                                           false, false,LeftOperand->TokenType,LeftOperand->subtype,"GF" );
                         operand2 = initOperand(operand2, RightOperand->tokenData.value,
                                           false, false,RightOperand->TokenType,RightOperand->subtype,"GF" );
                    }
                    else
                    {
                        operand1 = initOperand(operand1, LeftOperand->tokenData.value,
                                           false, false,LeftOperand->TokenType,LeftOperand->subtype,"LF" );
                        operand2 = initOperand(operand2, RightOperand->tokenData.value,
                                           false, false,RightOperand->TokenType,RightOperand->subtype,"GF" );
                    }
                    
                    instruction1op(&instList,POPS,operand2);
                    instruction1op(&instList,POPS,operand1);
                    instruction2op(&instList,INT2FLOAT,operand1,operand1);
                    instruction1op(&instList,PUSHS,operand1);
                    instruction1op(&instList,PUSHS,operand2);
                    return OK;
                }
                else if (LeftOperand->subtype ==sDoublePointNumber && RightOperand->subtype ==  sDoublePointNumber)
                {
                    return OK;
                }
                else 
                {
                    return ERR_RUN;
                }
                return OK;
                break;
         case sDivideInteger:
                if (LeftOperand->subtype ==sInteger && RightOperand->subtype == sInteger)
                {
                   return OK;
                }
                else 
                {
                    return ERR_RUN;
                }
        case sMultiplication:
        case sMinus:
                if (LeftOperand->subtype ==sInteger && RightOperand->subtype == sInteger)
                {
                    return OK;
                }
                else if (LeftOperand->subtype == sDoublePointNumber && RightOperand->subtype == sInteger)
                {
                    RightOperand->type = sDoublePointNumber;
                    /*PRETYPUJ*/
                    if(inMain)
                    {
                         operand1 = initOperand(operand1, RightOperand->tokenData.value,
                                           false, false,RightOperand->TokenType,RightOperand->subtype,"GF" );
                    }
                    else
                    {
                        operand1 = initOperand(operand1, RightOperand->tokenData.value,
                                           false, false,RightOperand->TokenType,RightOperand->subtype,"LF" );
                    }
                    
                    instruction1op(&instList,POPS,operand1);
                    instruction2op(&instList,INT2FLOAT,operand1,operand1);
                    instruction1op(&instList,PUSHS,operand1);
                    return OK;
                }
                else if (LeftOperand->subtype ==sInteger && RightOperand->subtype ==  sDoublePointNumber)
                {
                    LeftOperand->type = sDoublePointNumber;
                    /*PRETYPUJ*/
                   if(inMain)
                    {
                         operand1 = initOperand(operand1, LeftOperand->tokenData.value,
                                           false, false,LeftOperand->TokenType,LeftOperand->subtype,"GF" );
                         operand2 = initOperand(operand2, RightOperand->tokenData.value,
                                           false, false,RightOperand->TokenType,RightOperand->subtype,"GF" );
                    }
                    else
                    {
                        operand1 = initOperand(operand1, LeftOperand->tokenData.value,
                                           false, false,LeftOperand->TokenType,LeftOperand->subtype,"LF" );
                        operand2 = initOperand(operand2, RightOperand->tokenData.value,
                                           false, false,RightOperand->TokenType,RightOperand->subtype,"GF" );
                    }
                    
                    instruction1op(&instList,POPS,operand2);
                    instruction1op(&instList,POPS,operand1);
                    instruction2op(&instList,INT2FLOAT,operand1,operand1);
                    instruction1op(&instList,PUSHS,operand1);
                    instruction1op(&instList,PUSHS,operand2);
                    return OK;
                }
                else if (LeftOperand->subtype ==sDoublePointNumber && RightOperand->subtype ==  sDoublePointNumber)
                {
                    return OK;
                }
                else 
                {
                    return ERR_RUN;
                }
                break;


        default: 
            return ERR_SYN;
    }
    return OK;
}

int reduceBrackets(tStack *stack)
{
    if (!stackEmpty(stack))
    {
        tRedukToken* temp1 =stackTopPop(stack);
        tRedukToken* temp2 = stackTopPop(stack);
        tRedukToken* temp3 =  stackTopPop(stack);

        if (temp2->Redukuj == false && temp3->subtype == sLeftBracket)
        {
            
            if (stackTop(stack)->terminal == true)
            {
                IndexTerminalu = IndexOfTop(stack);
                stackIndex(stack,IndexTerminalu)->Redukuj = false;
            }
            stackPush(stack,temp2);
        }
        else 
        {
            stackPush(stack,temp3);
            stackPush(stack,temp2);
            stackPush(stack,temp1);
            return ERR_SYN;
        }
    }
    else 
        return ERR_SYN;

    return OK;
     
}