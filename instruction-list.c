/*
* Predmet  :   IFJ / IAL
* Súbor    :   instruction-list.c
* Projekt  :   Implementácia prekladača imperatívneho jazyka IFJ19
* Tým č    :   127
* Varianta :   I
* Autoři   : xhalom00, Ivan Halomi 
*            xhiner00, Martin Hiner
*            xsevci64, Adam Ševčík
*            xzakji02, Jiří Žák
*/
#include "instruction-list.h"
#include <stdbool.h>
#include <string.h>

extern tDLListInst instList;
tOperand operand1;
tOperand operand2; 
tOperand operand3;

///////////////////////////////////////////////Zoznam//////////////////////////////////////////////
void DLInitList (tDLListInst *L) {
    L->First = NULL;
    L->Last = NULL;
    L->Act = NULL;
    
}

void DLDisposeList (tDLListInst *L) {
    tDLElemPtr tDLElem = L->First;
	while (L->First != NULL)
	{
		L->First = L->First->rptr;
		free(tDLElem);
		tDLElem = L->First;
	}

	
	L->First = L->Last = L->Act = NULL;
}

void DLInsertFirst (tDLListInst *L, tInstr instruction) {
    // vytvoreni noveho prku a alokovanie pameti pre novy prvok
    tDLElemPtrInstruction newElem;
    if( (newElem = (struct tDLElem*)malloc(sizeof(struct tDLElem))) == NULL ) {
        return 0;
    }
    newElem->instruction = instruction;

    // ak je zoznam prazdny prvok pojde na zaciatok
    if (L->First == NULL) {
        L->First = newElem;
        L->Last = newElem;
        newElem->lptr = NULL;
        newElem->rptr = NULL;
    }
    else {
        tDLElemPtrInstruction elemToJoin = L->First; 
        L->First = newElem;
        newElem->lptr = NULL;
        newElem->rptr = elemToJoin;
        elemToJoin->lptr = newElem;
    }
    return 1;
}

void DLInsertLast(tDLListInst *L, tInstr instruction) {
    // vytvoreni noveho prku a alokovanie pameti pre novy prvok
    tDLElemPtrInstruction newElem;
    if( (newElem = (struct tDLElem*)malloc(sizeof(struct tDLElem))) == NULL ) {
        return 0;
    }
    newElem->instruction = instruction;

    // pokial je zoznam prazdny vlozime prvok na zaciatok = koniec
    if (L->First == NULL) {
        L->First = newElem;
        L->Last = newElem;
        newElem->lptr = NULL;
        newElem->rptr = NULL;
    }
    //pokial nieje prazdny vlozime prvok na koniec
    else {
        L->Last->rptr = newElem;
        newElem->lptr = L->Last;
        newElem->rptr = NULL;
        L->Last = newElem;
    }
   
}

void DLFirst (tDLListInst *L) {
    L->Act = L->First;
}

void DLLast (tDLListInst *L) {
    L->Act = L->Last;
}

void DLCopyFirst (tDLListInst *L, tInstr *instruction) {
    // kontrola ci je zoznam neprazdny
    if (L->First == NULL) {
        return;
    }
    //vracia data prveho prvku
    *instruction = L->First->instruction;
}

void DLCopyLast (tDLListInst *L, tInstr *instruction) {
    // kontrola ci je zoznam neprazdny
    if (L->Last == NULL) {
        return;
    }
    // vracia data posledneho prvku
    *instruction = L->Last->instruction;
}

void DLDeleteFirst (tDLListInst *L) {
    if (L->First) {
        //zrusenie aktivity prveho prvku
        if (L->Act == L->First)
            L->Act = NULL;
        // odstranenie prveho prvku
        L->First = L->First->rptr;
        free(L->First->lptr); 
        L->First->lptr = NULL;
    }
}

void DLDeleteLast (tDLListInst *L) {
    if (L->Last) {
        //ak je aktivny posledny prvok aktivita sa zrusi
        if (L->Act == L->Last)
            L->Act = NULL;
        //odstranenie posledneho prvku
        L->Last = L->Last->lptr; 
        free(L->Last->rptr); 
        L->Last->rptr = NULL;
    }
}

void DLPostDelete (tDLListInst *L) {
    //v zozname musi byt aktivny prvok ktory zaroven nieje posledny
    if (L->Act && L->Act != L->Last) {
        tDLElemPtrInstruction elemToDelete = L->Act->rptr;
        tDLElemPtrInstruction elemToJoin = elemToDelete->rptr;
    //posun na miesto vymzaneho prvku
        if (elemToJoin) {
            elemToJoin->lptr = L->Act;
            L->Act->rptr = elemToJoin;
        }
        else {
            L->Act->rptr = NULL;
            L->Last = L->Act;
        }
        //uvolnenie prvku
        free(elemToDelete);
    }
}

void DLPreDelete (tDLListInst *L) {
    //v zozname musi byt aktivny prvok ktory nieje prvy
    if (L->Act && L->Act != L->First) {
        tDLElemPtrInstruction elemToDelete = L->Act->lptr;
        tDLElemPtrInstruction elemToJoin = elemToDelete->lptr;
        //posun na miesto vymzaneho prvku
        if (elemToJoin) {
            elemToJoin->rptr = L->Act;
            L->Act->lptr = elemToJoin;
        }
        else {
            L->Act->lptr = NULL;
            L->First = L->Act;
        }
        //uvolnenie prvku
        free(elemToDelete);
    }
}

void DLPostInsert (tDLListInst *L, tInstr instruction) {
    if (L->Act) {
        // vytvorenie a alokacia noveho prvku na vlozenie
        tDLElemPtrInstruction newElem;
        if( (newElem = (struct tDLElem*)malloc(sizeof(struct tDLElem))) == NULL ) {
            return 0;
        }
        newElem->instruction = instruction;

        tDLElemPtrInstruction elemToJoin = L->Act->rptr;
        // vlozenie prvku do zoznamu
        if (elemToJoin == NULL) {
            L->Act->rptr = newElem;
            newElem->lptr = L->Act;
            newElem->rptr = NULL;
            L->Last = newElem;
        }
        else { // vlozenie prvku mezi act a elemToJoin
            newElem->lptr = L->Act;
            newElem->rptr = elemToJoin;
            L->Act->rptr = newElem;
            elemToJoin->lptr = newElem;
        }
    }
   
}

void DLPreInsert (tDLListInst *L, tInstr instruction) {
    if (L->Act) {
        // vytvorenie a alokacia noveho prvku na vlozenie
        tDLElemPtrInstruction newElem;
        if( (newElem = (struct tDLElem*)malloc(sizeof(struct tDLElem))) == NULL ) {
            return 0;
        }
        newElem->instruction = instruction;

        tDLElemPtrInstruction elemToJoin = L->Act->lptr;
        //vlozenie prvku do zoznamu
        if (elemToJoin == NULL) {
            L->Act->lptr = newElem;
            newElem->rptr = L->Act;
            newElem->lptr = NULL;
            L->First = newElem;
        }
        else { // vlozenie prvku medzi Act a elemToJoin
            newElem->rptr = L->Act;
            newElem->lptr = elemToJoin;
            L->Act->lptr = newElem;
            elemToJoin->rptr = newElem;
        }
    }
   
}

void DLCopy (tDLListInst *L, tInstr *instruction) {
    //kontrola ci je zoznam aktivny
    if (L->Act == NULL) {
        return 0;
    }
  //vrati data aktivneho prvku
    *instruction = L->Act->instruction;
    
}

void DLActualize (tDLListInst *L, tInstr instruction) {
    // pokial je zoznam aktivy aktualizuje data aktivneho prvku
    if (L->Act) {
        L->Act->instruction = instruction;
    }
}

void DLSucc (tDLListInst *L) {
    // ak je zoznam aktivny posunie aktivitu na nasledujuci
    if (L->Act) {
        L->Act = L->Act->rptr;
    }
}

void DLPred (tDLListInst *L) {
    // ak je zoznam aktivny vrati aktivitu na predchadzajuci
    if (L->Act) {
        L->Act = L->Act->lptr;
    }
}

int DLActive (tDLListInst *L) {
    //ak je zoznam aktivny vrati nenulovu hodnotu
    return(L->Act != NULL);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void instructionGenerator(tDLListInst *L, int instType, char* a1, char* a2, char* a3) {
    tInstr instr;
    instruction.instType = instType;
    instruction.a1 = a1;
    instruction.a2 = a2;
    instruction.a3 = a3;
    
    DLInsertLast(L,instr);
}

tOperand initOperand(tOperand operand, char* value, bool Label, bool tmp, int type, int subtype, char* frame){
    operand.value = value;
    operand.tmp = tmp;
    operand.Label = Label
    operand.type = type;
    operand.subtype = subtype;
    operand.frame = frame;
    return operand;
}
//instrukcia bez operandov
void instuction0op (tDLListInst *L, int Type){
    instructionGenerator(L , Type, NULL, NULL, NULL);
}
//instrukcia s 1 operandom/////////////////////////////////////////////////////////////

void instruction1op (tDLListInst *L, int Type, tOperand operand1){
    char* _operand1 = "";
//premenna identifikator
    if(operand1.type == sIdentifikator){
        if(strcmp(operand1.frame,"GF") == 0){
            strcat(_operand1,"GF@");
            strcat(_operand1,operand1.value);
        }
        else if(strcmp(operand1.frame,"LF") == 0){
            strcat(_operand1,"LF@");
            strcat(_operand1,operand1.value);
        }
        else if(strcmp(operand1.frame,"TF") == 0){
            strcat(_operand1,"TF@");
            strcat(_operand1,operand1.value);
        }
        if(operand1.tmp == true){
            strcat(_operand1,"$tmp");
        }
        if(operand1.Label == true){
            strcat("$",_operand1);
        }
    }
//konstatna
    else if(operand1.type == sString){
            strcat(_operand1,"string@");
            strcat(_operand1,operand1.value);
    }
    else if(operand1.type == sNumber){
        if(operand1.subtype == sInteger){
            strcat(_operand1,"int@");
            strcat(_operand1,operand1.value);
        }
        else if(operand1.subtype == sDoublePointNumber || operand1.subtype == sDoubleExponentNumber){
            strcat(_operand1,"float@");
            strcat(_operand1,operand1.value);
        }
    }
    else if(operand1.subtype == sBool){
            strcat(_operand1,"bool@");
            strcat(_operand1,operand1.value);
    }
    else if(operand1.subtype == sNil){
            strcat(_operand1,"nil@");
            strcat(_operand1,operand1.value);
    }
    instructionGenerator(L , Type, _operand1, "", "");
}

//instrukcia s 2 operandmi//////////////////////////////////////////////////////////

void instruction2op (tDLListInst *L, int Type, tOperand operand1, tOperand operand2){
//operand 1
    char* _operand1 = "";
//premenna identifikator
    if(operand1.type == sIdentifikator){
        if(strcmp(operand1.frame,"GF") == 0){
            strcat(_operand1,"GF@");
            strcat(_operand1,operand1.value);
        }
        else if(strcmp(operand1.frame,"LF") == 0){
            strcat(_operand1,"LF@");
            strcat(_operand1,operand1.value);
        }
        else if(strcmp(operand1.frame,"TF") == 0){
            strcat(_operand1,"TF@");
            strcat(_operand1,operand1.value);
        }
        if(operand1.tmp == true){
            strcat(_operand1,"$tmp");
        }
        if(operand1.Label == true){
            strcat("$",_operand1);
        }
    }
//konstatna
    else if(operand1.type == sString){
            strcat(_operand1,"string@");
            strcat(_operand1,operand1.value);
    }
    else if(operand1.type == sNumber){
        if(operand1.subtype == sInteger){
            strcat(_operand1,"int@");
            strcat(_operand1,operand1.value);
        }
        else if(operand1.subtype == sDoublePointNumber || operand1.subtype == sDoubleExponentNumber){
            strcat(_operand1,"float@");
            strcat(_operand1,operand1.value);
        }
    }
    else if(operand1.subtype == sBool){
            strcat(_operand1,"bool@");
            strcat(_operand1,operand1.value);
    }
    else if(operand1.subtype == sNil){
            strcat(_operand1,"nil@");
            strcat(_operand1,operand1.value);
    }
//operand 2

        char* _operand2 = "";
//premenna identifikator
     if(operand2.type == sIdentifikator){
        if(strcmp(operand2.frame,"GF") == 0){
            strcat(_operand2,"GF@");
            strcat(_operand2,operand2.value);
        }
        else if(strcmp(operand2.frame,"LF") == 0){
            strcat(_operand2,"LF@");
            strcat(_operand2,operand2.value);
        }
        else if(strcmp(operand2.frame,"TF") == 0){
            strcat(_operand2,"TF@");
            strcat(_operand2,operand2.value);
        }
        if(operand2.tmp == true){
            strcat(_operand2,"$tmp");
        }
        if(operand2.Label == true){
            strcat("$",_operand2);
        }
    }
//konstatna
    else if(operand2.type == sString){
            strcat(_operand2,"string@");
            strcat(_operand2,operand2.value);
    }
    else if(operand2.type == sNumber){
        if(operand2.subtype == sInteger){
            strcat(_operand2,"int@");
            strcat(_operand2,operand2.value);
        }
        else if(operand2.subtype == sDoublePointNumber || operand2.subtype == sDoubleExponentNumber){
            strcat(_operand2,"float@");
            strcat(_operand2,operand1.value);
        }
    }
    else if(operand2.subtype == sBool){
            strcat(_operand2,"bool@");
            strcat(_operand2,operand2.value);
    }
    else if(operand2.subtype == sNil){
            strcat(_operand2,"nil@");
            strcat(_operand2,operand2.value);
    }
instructionGenerator(L , Type, _operand1, _operand2, "");
}
//instrukcia s 3 operandmi ///////////////////////////////////////////////////////////////
void instruction3op (tDLListInst *L, int Type, tOperand operand1, tOperand operand2, tOperand operand3){
//operand 1
    char* _operand1 = "";
//premenna identifikator
    if(operand1.type == sIdentifikator){
        if(strcmp(operand1.frame,"GF") == 0){
            strcat(_operand1,"GF@");
            strcat(_operand1,operand1.value);
        }
        else if(strcmp(operand1.frame,"LF") == 0){
            strcat(_operand1,"LF@");
            strcat(_operand1,operand1.value);
        }
        else if(strcmp(operand1.frame,"TF") == 0){
            strcat(_operand1,"TF@");
            strcat(_operand1,operand1.value);
        }
        if(operand1.tmp == true){
            strcat(_operand1,"$tmp");
        }
        if(operand1.Label == true){
            strcat("$",_operand1);
        }
    }
//konstatna
    else if(operand1.type == sString){
            strcat(_operand1,"string@");
            strcat(_operand1,operand1.value);
    }
    else if(operand1.type == sNumber){
        if(operand1.subtype == sInteger){
            strcat(_operand1,"int@");
            strcat(_operand1,operand1.value);
        }
        else if(operand1.subtype == sDoublePointNumber || operand1.subtype == sDoubleExponentNumber){
            strcat(_operand1,"float@");
            strcat(_operand1,operand1.value);
        }
    }
    else if(operand1.subtype == sBool){
            strcat(_operand1,"bool@");
            strcat(_operand1,operand1.value);
    }
    else if(operand1.subtype == sNil){
            strcat(_operand1,"nil@");
            strcat(_operand1,operand1.value);
    }
//operand 2

        char* _operand2 = "";
//premenna identifikator
     if(operand2.type == sIdentifikator){
        if(strcmp(operand2.frame,"GF") == 0){
            strcat(_operand2,"GF@");
            strcat(_operand2,operand2.value);
        }
        else if(strcmp(operand2.frame,"LF") == 0){
            strcat(_operand2,"LF@");
            strcat(_operand2,operand2.value);
        }
        else if(strcmp(operand2.frame,"TF") == 0){
            strcat(_operand2,"TF@");
            strcat(_operand2,operand2.value);
        }
        if(operand2.tmp == true){
            strcat(_operand2,"$tmp");
        }
        if(operand2.Label == true){
            strcat("$",_operand2);
        }
    }
//konstatna
    else if(operand2.type == sString){
            strcat(_operand2,"string@");
            strcat(_operand2,operand2.value);
    }
    else if(operand2.type == sNumber){
        if(operand2.subtype == sInteger){
            strcat(_operand2,"int@");
            strcat(_operand2,operand2.value);
        }
        else if(operand2.subtype == sDoublePointNumber || operand2.subtype == sDoubleExponentNumber){
            strcat(_operand2,"float@");
            strcat(_operand2,operand1.value);
        }
    }
    else if(operand2.subtype == sBool){
            strcat(_operand2,"bool@");
            strcat(_operand2,operand2.value);
    }
    else if(operand2.subtype == sNil){
            strcat(_operand2,"nil@");
            strcat(_operand2,operand2.value);
    }

//operand 3
    char* _operand3 = "";
//premenna identifikator
    if(operand3.type == sIdentifikator){
        if(strcmp(operand3.frame,"GF") == 0){
            strcat(_operand3,"GF@");
            strcat(_operand3,operand3.value);
        }
        else if(strcmp(operand3.frame,"LF") == 0){
            strcat(_operand3,"LF@");
            strcat(_operand3,operand3.value);
        }
        else if(strcmp(operand3.frame,"TF") == 0){
            strcat(_operand3,"TF@");
            strcat(_operand3,operand3.value);
        }
        if(operand3.tmp == true){
            strcat(_operand3,"$tmp");
        }
        if(operand3.Label == true){
            strcat("$",_operand3);
        }
    }
//konstatna
    else if(operand3.type == sString){
            strcat(_operand3,"string@");
            strcat(_operand3,operand3.value);
    }
    else if(operand3.type == sNumber){
        if(operand3.subtype == sInteger){
            strcat(_operand3,"int@");
            strcat(_operand3,operand3.value);
        }
        else if(operand3.subtype == sDoublePointNumber || operand3.subtype == sDoubleExponentNumber){
            strcat(_operand3,"float@");
            strcat(_operand3,operand3.value);
        }
    }
    else if(operand3.subtype == sBool){
            strcat(_operand3,"bool@");
            strcat(_operand3,operand3.value);
    }
    else if(operand3.subtype == sNil){
            strcat(_operand3,"nil@");
            strcat(_operand3,operand3.value);
    
instructionGenerator(L , Type, _operand1, _operand2, _operand3);
}


