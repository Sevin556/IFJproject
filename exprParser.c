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


extern tSymtable gTable;            //GL tabulka symbolů
extern tSymtable lTable;            //Lokální tabulka
extern bool inMain;         //Indikátor, že se kontroluje tělo funkce
extern tDLListInst instList; 
extern tBSTNodePtr node;             //uzol kam vraciam typ hodnoty

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


    if (type == sDollar)
    {
        return 13;
    }
    if (type == sIdentificator || type == sNumber || type == sString)
    {
        return 14;
    }    
    
    int arr[15] = {sPlus,sMinus,sMultiplication,sDivideInteger,sDivideFloat,sEqual,sLess,sMore,sMoreEqual,sLessEqual,
				sInequal,sLeftBracket,sRightBracket,sEOL,sColon};
        for (int i = 0; i < 15 ; i++ )
         {
             if (subtype == arr[i] || type == arr[i]) {
                if (i==14)
                {
                    i =13;
                }
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
    int ret = OK;                            //return value
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
    stackPush(&exprStack,firstToken);
    IndexTerminalu = 0;
    tToken *actToken;

    if (dostanyToken == NULL)
    {
        
       actToken = get_token();
       if (actToken->type == sLexError )
        return ERR_LEX;
    }
    else 
    {
        actToken = dostanyToken;
        if (actToken->type == sLexError )
            return ERR_LEX;
    }

    ret =shiftToStack(&exprStack,actToken);
    if (ret != OK){
        return ret;
    }
    actToken = get_token();
    if (actToken->type == sLexError ){
        return ERR_LEX;
    }
        
        
    tRedukToken* temp;
    while (1)
    {
        if (actToken->type == sLexError )
            return ERR_LEX;
        
        temp=stackIndex(&exprStack,IndexTerminalu);
        if (temp == NULL)
        {
            return ERR_INTERN;
        }
       int indexInTable = tableIndexSelect(temp->TokenType,temp->subtype);
       switch (Table[indexInTable][tableIndexSelect(actToken->type,actToken->subtype)])
        {
        case L:
            ret = shiftToStack(&exprStack,actToken);
            if (ret != OK)
                return ret;
            actToken = get_token();
            if (actToken->type == sLexError )
                return ERR_LEX;
            break;
        case H:
            ret = ApplyRule(&exprStack);
            if (ret != OK){
                return ret;
            }
                
            break;
        case EQUAL://len pri redukovani zatvoriek
            shiftToStack(&exprStack,actToken);
            ret = reduceBrackets(&exprStack);
             if (ret != OK) {
                    return ret;
                }
            actToken = get_token();
            if (actToken->type == sLexError )
                return ERR_LEX;
            if (actToken == NULL)
            {
                return ERR_SYN;
            }
            
            break;
        case ERROR:                
            return ERR_SYN;
        case EXITPARSE:
            if (node != NULL)
            ((tVariable *) node->Data)->retType = stackTop(&exprStack)->subtype;
            return OK;
        default:
            //printf("robim default");
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
    {
        return NULL;
    }
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
        tRedukToken* temp =stackIndex(stack,IndexTerminalu);
        temp->Redukuj= true;
            
        tRedukToken* new_token = createNewToken(token);
        if (new_token == NULL)
            return ERR_INTERN;

        if (new_token->TokenType  == sIdentificator)
        {    tBSTNodePtr ID_uzlu =symTableSearch(&lTable,new_token->tokenData);
            
            if ( ID_uzlu != NULL)
            {
                if(!inMain)//ak sme ju nasli v globalnej a musi byt aj v lokalnej
                {
                    ID_uzlu = symTableSearch(&lTable,new_token->tokenData);
                    if ( ID_uzlu == NULL)
                    {
                        return ERR_SEM_VAR;
                    }
                    else 
                    {
                        /*ULOZ JEJ TYP*/
                       new_token->type = ((tVariable *) ID_uzlu->Data)->retType ;
                        new_token->subtype = ((tVariable *) ID_uzlu->Data)->retType ;
                    }
                }
                else 
                return ERR_SEM_VAR;
            }
            else  
            {
                if(!inMain)
                {
                    ID_uzlu = symTableSearch(&lTable,new_token->tokenData);
                    if ( ID_uzlu == NULL){
                        return ERR_SEM_VAR;
                    }
                    else 
                    {
                        /*ULOZ JEJ TYP*/
                        new_token->type = ((tVariable *) ID_uzlu->Data)->retType ;
                        new_token->subtype = ((tVariable *) ID_uzlu->Data)->retType ;
                    }
                }
                else 
                return ERR_SEM_VAR;
            }


        }
        //printf("EXPR pushujem %s \n",new_token->tokenData.value);
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
        //printf("EXPR aplikujem pravidlo pre %s \n",temp->tokenData.value);
        switch(temp->TokenType)
        {   //situacia pre operandy
            case sDollar: return OK;
            case sIdentificator:
            case sNumber:
            case sString:
                                 //printf("EXPR aplikujem pravidlo sIDEN pre %s \n",temp->tokenData.value);
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
                             //printf("EXPR aplikujem pravidlo sOPERATOR pre %s \n",temp->tokenData.value);
                            switch(temp->subtype)
                                {
                                    case sPlus:
                                           ret=checkOperator(stack,temp->subtype);
                                            if (ret!= OK)
                                            return ret;
                                            /*JE TO ZLOZITEJSIE DOPLN KOD 
                                            vygeneruj kod */
                                            instruction0op(&instList,ADDS);
                                            break;
                                    case sMultiplication:
                                            ret=checkOperator(stack,temp->subtype);
                                            if (ret!= OK)
                                            return ret;
                                            instruction0op(&instList,MULS);

                                            /*vygeneruj kod */
                                            break;
                                    case sDivideFloat:
                                            //printf("EXPR robim sDivideFloat");
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
                                    //printf("NEMAM PRAVIDLO");
                                    return ERR_SYN;
                                }
                            break;
        }
        stackPop(stack);
        stackPop(stack);
        IndexTerminalu = IndexOfTop(stack)-1;
        temp = stackIndex(stack,IndexTerminalu);
        temp->Redukuj = false;

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
    int ret = OK;                            //return value
    //printf("EXPR zacinam CheckOperator s %d \n",znamienko);
    if (!stackEmpty(stack))
    {
        tRedukToken* RightOperand= stackTopPop(stack);
        //printf("EXPR pravy operator je  %s \n",RightOperand->tokenData.value);
        if (RightOperand->TableIndex != 14)
        return ERR_SYN;

        tRedukToken* operator = stackTopPop(stack);
        //printf("EXPR  operator je  %s \n",operator->tokenData.value);
        tRedukToken* LeftOperand = stackTopPop(stack);
        if (LeftOperand->TableIndex != 14)
            return ERR_SYN;

        //printf("EXPR lavy operator %s \n",LeftOperand->tokenData.value);
        
        
        if (LeftOperand->TableIndex != 14 || LeftOperand->terminal !=false ||
            operator->terminal != true || operator->TokenType != sOperand || 
            RightOperand->terminal !=false || RightOperand->TableIndex !=14)
        {
            return ERR_SYN;
        }
        ret = checkSemantic(LeftOperand,RightOperand,znamienko);
        if (ret !=OK )
        {
            return ret;
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
    //printf("EXPR zacinam semantiku s %s a %s",RightOperand->tokenData.value,LeftOperand->tokenData.value);
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
                    
                    /*PRETYPUJ*/
                   if(inMain)
                    {
                         operand1 = initOperand(operand1, "tmp",
                                           false,true,sIdentificator,-1,"GF" );
                    }
                    else
                    {
                        operand1 = initOperand(operand1, "tmp",
                                           false,true,sIdentificator,-1,"LF" );
                    }
                    
                    instruction1op(&instList,POPS,operand1);
                    instruction2op(&instList,INT2FLOAT,operand1,operand1);
                    RightOperand->subtype = sDoublePointNumber;
                    instruction1op(&instList,PUSHS,operand1);


                    return OK;
                }
                else if (LeftOperand->subtype ==sInteger && RightOperand->subtype ==  sDoublePointNumber)
                {
                    
                    
                    if(inMain)
                    {
                         operand1 = initOperand(operand1, "tmp",
                                           false,true,sIdentificator,-1,"GF" );
                         operand2 = initOperand(operand2, "tmp2",
                                           false,true,sIdentificator,-1,"GF" );
                    }
                    else
                    {
                        operand1 = initOperand(operand1, "tmp",
                                           false,true,sIdentificator,-1,"LF" );
                        operand2 = initOperand(operand2, "tmp2",
                                           false,true,sIdentificator,-1,"LF" );
                    }
                    
                    instruction1op(&instList,POPS,operand2);
                    instruction1op(&instList,POPS,operand1);
                    instruction2op(&instList,INT2FLOAT,operand1,operand1);
                    LeftOperand->subtype = sDoublePointNumber;
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
                if (strcmp(RightOperand->tokenData.value,"0") == 0){
                    return ERR_ZERO;
                }
                if (LeftOperand->subtype ==sInteger && RightOperand->subtype == sInteger)
                {   
                    
                   /*PRETYPUJ OBA*/
                   if(inMain)
                    {
                         operand1 = initOperand(operand1, "tmp",
                                           false,true,sIdentificator,-1,"GF" );
                         operand2 = initOperand(operand2, "tmp2",
                                           false,true,sIdentificator,-1,"GF" );
                    }
                    else
                    {
                        operand1 = initOperand(operand1, "tmp",
                                           false,true,sIdentificator,-1,"LF" );
                        operand2 = initOperand(operand2, "tmp2",
                                           false,true,sIdentificator,-1,"LF" );
                    }
                    
                    instruction1op(&instList,POPS,operand1);
                    instruction2op(&instList,INT2FLOAT,operand1,operand1);
                    instruction1op(&instList,POPS,operand2);
                    instruction2op(&instList,INT2FLOAT,operand2,operand2);
                    LeftOperand->subtype = sDoublePointNumber;
                    RightOperand->subtype = sDoublePointNumber;
                    instruction1op(&instList,PUSHS,operand2);
                    instruction1op(&instList,PUSHS,operand1);
                    //printf("EXPR pretypujem oba ");

                    return OK;
                }
                else if (LeftOperand->subtype == sDoublePointNumber && RightOperand->subtype == sInteger)
                {
                    

                    /*PRETYPUJ*/
                    if(inMain)
                    {
                         operand1 = initOperand(operand1, "tmp",
                                           false,true,sIdentificator,-1,"GF" );
                    }
                    else
                    {
                        operand1 = initOperand(operand1, "tmp",
                                           false,true,sIdentificator,-1,"LF" );
                    }
                    
                    instruction1op(&instList,POPS,operand1);
                    instruction2op(&instList,INT2FLOAT,operand1,operand1);
                    RightOperand->subtype = sDoublePointNumber;
                    instruction1op(&instList,PUSHS,operand1);
                    return OK;
                }
                else if (LeftOperand->subtype ==sInteger && RightOperand->subtype ==  sDoublePointNumber)
                {
                    

                    /*PRETYPUJ*/
                     if(inMain)
                    {
                         operand1 = initOperand(operand1, "tmp",
                                           false,true,sIdentificator,-1,"GF" );
                         operand2 = initOperand(operand2, "tmp2",
                                           false,true,sIdentificator,-1,"GF" );
                    }
                    else
                    {
                        operand1 = initOperand(operand1, "tmp",
                                           false,true,sIdentificator,-1,"LF" );
                        operand2 = initOperand(operand2, "tmp2",
                                           false,true,sIdentificator,-1,"LF" );
                    }
                    
                    instruction1op(&instList,POPS,operand2);
                    instruction1op(&instList,POPS,operand1);
                    instruction2op(&instList,INT2FLOAT,operand1,operand1);
                    LeftOperand->subtype = sDoublePointNumber;
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
                if (strcmp(RightOperand->tokenData.value,"0") == 0){
                    return ERR_ZERO;
                }
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
                   
                    /*PRETYPUJ*/
                    if(inMain)
                    {
                         operand1 = initOperand(operand1, "tmp",
                                           false,true,sIdentificator,-1,"GF" );
                    }
                    else
                    {
                        operand1 = initOperand(operand1, "tmp",
                                           false,true,sIdentificator,-1,"LF" );
                    }
                    
                    instruction1op(&instList,POPS,operand1);
                    instruction2op(&instList,INT2FLOAT,operand1,operand1);
                     RightOperand->subtype = sDoublePointNumber;
                    instruction1op(&instList,PUSHS,operand1);
                    return OK;
                }
                else if (LeftOperand->subtype ==sInteger && RightOperand->subtype ==  sDoublePointNumber)
                {
                    
                    /*PRETYPUJ*/
                   if(inMain)
                    {
                         operand1 =initOperand(operand1, "tmp",
                                           false,true,sIdentificator,-1,"GF" );
                         operand2 = initOperand(operand2, "tmp2",
                                           false,true,sIdentificator,-1,"GF" );
                    }
                    else
                    {
                        operand1 = initOperand(operand1, "tmp",
                                           false,true,sIdentificator,-1,"LF" );
                        operand2 = initOperand(operand2, "tmp2",
                                           false,true,sIdentificator,-1,"LF" );
                    }
                    
                    instruction1op(&instList,POPS,operand2);
                    instruction1op(&instList,POPS,operand1);
                    instruction2op(&instList,INT2FLOAT,operand1,operand1);
                    LeftOperand->subtype = sDoublePointNumber;
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
                tRedukToken* temp4=stackIndex(stack,IndexTerminalu);
                temp4->Redukuj = false;
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
