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

///////////////////////////////////////////////Zoznam//////////////////////////////////////////////
void DLInitList (tDLList *L) {
    L->First = NULL;
    L->Last = NULL;
    L->Act = NULL;
    
}

void DLDisposeList (tDLList *L) {
    tDLElemPtr tDLElem = L->First;
	while (L->First != NULL)
	{
		L->First = L->First->rptr;
		free(tDLElem);
		tDLElem = L->First;
	}

	
	L->First = L->Last = L->Act = NULL;
}

void DLInsertFirst (tDLList *L, tInstr instruction) {
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

void DLInsertLast(tDLList *L, tInstr instruction) {
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

void DLFirst (tDLList *L) {
    L->Act = L->First;
}

void DLLast (tDLList *L) {
    L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, tInstr *instruction) {
    // kontrola ci je zoznam neprazdny
    if (L->First == NULL) {
        return;
    }
    //vracia data prveho prvku
    *instruction = L->First->instruction;
}

void DLCopyLast (tDLList *L, tInstr *instruction) {
    // kontrola ci je zoznam neprazdny
    if (L->Last == NULL) {
        return;
    }
    // vracia data posledneho prvku
    *instruction = L->Last->instruction;
}

void DLDeleteFirst (tDLList *L) {
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

void DLDeleteLast (tDLList *L) {
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

void DLPostDelete (tDLList *L) {
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

void DLPreDelete (tDLList *L) {
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

void DLPostInsert (tDLList *L, tInstr instruction) {
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

void DLPreInsert (tDLList *L, tInstr instruction) {
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

void DLCopy (tDLList *L, tInstr *instruction) {
    //kontrola ci je zoznam aktivny
    if (L->Act == NULL) {
        return 0;
    }
  //vrati data aktivneho prvku
    *instruction = L->Act->instruction;
    
}

void DLActualize (tDLList *L, tInstr instruction) {
    // pokial je zoznam aktivy aktualizuje data aktivneho prvku
    if (L->Act) {
        L->Act->instruction = instruction;
    }
}

void DLSucc (tDLList *L) {
    // ak je zoznam aktivny posunie aktivitu na nasledujuci
    if (L->Act) {
        L->Act = L->Act->rptr;
    }
}

void DLPred (tDLList *L) {
    // ak je zoznam aktivny vrati aktivitu na predchadzajuci
    if (L->Act) {
        L->Act = L->Act->lptr;
    }
}

int DLActive (tDLList *L) {
    //ak je zoznam aktivny vrati nenulovu hodnotu
    return(L->Act != NULL);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void instructionGenerator(tDLList *L, int instType, void *a1, void *a2, void *a3) {
    tInstr instr;
    instruction.instType = instType;
    instruction.a1 = a1;
    instruction.a2 = a2;
    instruction.a3 = a3;
    
    DLInsertLast(L,instr);
}

tOperand initOperand(tOperand operand, char* value, char* type, char* frame){
    operand.value = value;
    operand.type = type;
    operand.frame = frame;
    return operand;
}

void instuction0op (tDLList *L, int Type){
    instructionGenerator(L , Type, NULL, NULL, NULL);
}

