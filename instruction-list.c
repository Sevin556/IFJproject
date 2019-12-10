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

extern tDLListInst instList;
tOperand operand1;
tOperand operand2;
tOperand operand3;

tInstr printinst;

///////////////////////////////////////////////Zoznam//////////////////////////////////////////////
void DLInitList(tDLListInst *L) {
    L->First = NULL;
    L->Last = NULL;
    L->Act = NULL;
    functionChr();
    functionLen();
    functionOrd();
}

void DLDisposeList(tDLListInst *L) {
    tDLElemPtrInstruction tDLElem = L->First;
    while (L->First != NULL) {
        L->First = L->First->rptr;
        free(tDLElem);
        tDLElem = L->First;
    }

    L->First = L->Last = L->Act = NULL;
}

int DLInsertFirst(tDLListInst *L, tInstr instruction) {
    // vytvoreni noveho prku a alokovanie pameti pre novy prvok
    tDLElemPtrInstruction newElem;
    if ((newElem = (struct tDLElem *) malloc(sizeof(struct tDLElem))) == NULL) {
        return 0;
    }
    newElem->instruction = instruction;

    // ak je zoznam prazdny prvok pojde na zaciatok
    if (L->First == NULL) {
        L->First = newElem;
        L->Last = newElem;
        newElem->lptr = NULL;
        newElem->rptr = NULL;
    } else {
        tDLElemPtrInstruction elemToJoin = L->First;
        L->First = newElem;
        newElem->lptr = NULL;
        newElem->rptr = elemToJoin;
        elemToJoin->lptr = newElem;
    }
    return 0;
}

int DLInsertLast(tDLListInst *L, tInstr instruction) {
    // vytvoreni noveho prku a alokovanie pameti pre novy prvok
    tDLElemPtrInstruction newElem;
    if ((newElem = (struct tDLElem *) malloc(sizeof(struct tDLElem))) == NULL) {
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
    return 0;
}

void DLFirst(tDLListInst *L) {
    L->Act = L->First;
}

void DLLast(tDLListInst *L) {
    L->Act = L->Last;
}

void DLCopyFirst(tDLListInst *L, tInstr *instruction) {
    // kontrola ci je zoznam neprazdny
    if (L->First == NULL) {
        return;
    }
    //vracia data prveho prvku
    *instruction = L->First->instruction;
}

void DLCopyLast(tDLListInst *L, tInstr *instruction) {
    // kontrola ci je zoznam neprazdny
    if (L->Last == NULL) {
        return;
    }
    // vracia data posledneho prvku
    *instruction = L->Last->instruction;
}

void DLDeleteFirst(tDLListInst *L) {
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

void DLDeleteLast(tDLListInst *L) {
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

void DLPostDelete(tDLListInst *L) {
    //v zozname musi byt aktivny prvok ktory zaroven nieje posledny
    if (L->Act && L->Act != L->Last) {
        tDLElemPtrInstruction elemToDelete = L->Act->rptr;
        tDLElemPtrInstruction elemToJoin = elemToDelete->rptr;
        //posun na miesto vymzaneho prvku
        if (elemToJoin) {
            elemToJoin->lptr = L->Act;
            L->Act->rptr = elemToJoin;
        } else {
            L->Act->rptr = NULL;
            L->Last = L->Act;
        }
        //uvolnenie prvku
        free(elemToDelete);
    }
}

void DLPreDelete(tDLListInst *L) {
    //v zozname musi byt aktivny prvok ktory nieje prvy
    if (L->Act && L->Act != L->First) {
        tDLElemPtrInstruction elemToDelete = L->Act->lptr;
        tDLElemPtrInstruction elemToJoin = elemToDelete->lptr;
        //posun na miesto vymzaneho prvku
        if (elemToJoin) {
            elemToJoin->rptr = L->Act;
            L->Act->lptr = elemToJoin;
        } else {
            L->Act->lptr = NULL;
            L->First = L->Act;
        }
        //uvolnenie prvku
        free(elemToDelete);
    }
}

int DLPostInsert(tDLListInst *L, tInstr instruction) {
    if (L->Act) {
        // vytvorenie a alokacia noveho prvku na vlozenie
        tDLElemPtrInstruction newElem;
        if ((newElem = (struct tDLElem *) malloc(sizeof(struct tDLElem))) == NULL) {
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
        } else { // vlozenie prvku mezi act a elemToJoin
            newElem->lptr = L->Act;
            newElem->rptr = elemToJoin;
            L->Act->rptr = newElem;
            elemToJoin->lptr = newElem;
        }
    }
    return 0;
}

int DLPreInsert(tDLListInst *L, tInstr instruction) {
    if (L->Act) {
        // vytvorenie a alokacia noveho prvku na vlozenie
        tDLElemPtrInstruction newElem;
        if ((newElem = (struct tDLElem *) malloc(sizeof(struct tDLElem))) == NULL) {
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
        } else { // vlozenie prvku medzi Act a elemToJoin
            newElem->rptr = L->Act;
            newElem->lptr = elemToJoin;
            L->Act->lptr = newElem;
            elemToJoin->rptr = newElem;
        }
    }
    return 0;
}

int DLCopy(tDLListInst *L, tInstr *instruction) {
    //kontrola ci je zoznam aktivny
    if (L->Act == NULL) {
        return 0;
    }
    //vrati data aktivneho prvku
    *instruction = L->Act->instruction;
    return 0;
}

void DLActualize(tDLListInst *L, tInstr instruction) {
    // pokial je zoznam aktivy aktualizuje data aktivneho prvku
    if (L->Act) {
        L->Act->instruction = instruction;
    }
}

void DLSucc(tDLListInst *L) {
    // ak je zoznam aktivny posunie aktivitu na nasledujuci
    if (L->Act) {
        L->Act = L->Act->rptr;
    }
}

void DLPred(tDLListInst *L) {
    // ak je zoznam aktivny vrati aktivitu na predchadzajuci
    if (L->Act) {
        L->Act = L->Act->lptr;
    }
}

int DLActive(tDLListInst *L) {
    //ak je zoznam aktivny vrati nenulovu hodnotu
    return (L->Act != NULL);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

void instructionGenerator(tDLListInst *L, int instType, char *o1, char *o2, char *o3) {
    tInstr instruction;
    instruction.Type = instType;
    instruction.o1 = o1;
    instruction.o2 = o2;
    instruction.o3 = o3;

    DLInsertLast(L, instruction);
}


tOperand initOperand(tOperand operand, char *value, bool Label, bool tmp, int type, int subtype, char *frame) {
    operand.value = value;
    operand.tmp = tmp;
    operand.Label = Label;
    operand.type = type;
    operand.subtype = subtype;
    operand.frame = frame;
    return operand;
}

//instrukcia bez operandov
void instruction0op(tDLListInst *L, int Type) {
    instructionGenerator(L, Type, NULL, NULL, NULL);
}
//instrukcia s 1 operandom/////////////////////////////////////////////////////////////

void instruction1op(tDLListInst *L, int Type, tOperand operand1) {
    char *_operand1 = malloc(sizeof(char) * 100);
    if(_operand1 == NULL)
        return;
//premenna identifikator
    if (operand1.type == sIdentificator) {
        if (operand1.Label == true) {
            strcat(_operand1, "$");
            strcat(_operand1, operand1.value);
        } else if (strcmp(operand1.frame, "GF") == 0) {
            strcat(_operand1, "GF@");
            strcat(_operand1, operand1.value);
        } else if (strcmp(operand1.frame, "LF") == 0) {
            strcat(_operand1, "LF@");
            strcat(_operand1, operand1.value);
        } else if (strcmp(operand1.frame, "TF") == 0) {
            strcat(_operand1, "TF@");
            strcat(_operand1, operand1.value);
        }
        if (operand1.tmp == true) {
            strcat(_operand1, "$tmp");
        }
    }
//konstatna
    else if (operand1.type == sString) {
        strcat(_operand1, "string@");
        strcat(_operand1, operand1.value);
    } else if (operand1.type == sNumber) {
        if (operand1.subtype == sInteger) {
            strcat(_operand1, "int@");
            strcat(_operand1, operand1.value);
        } else if (operand1.subtype == sDoublePointNumber || operand1.subtype == sDoubleExponentNumber) {
            strcat(_operand1, "float@");
            strcat(_operand1, operand1.value);
        }
    }
    /*else if(operand1.subtype == sBool){
            strcat(_operand1,"bool@");
            strcat(_operand1,operand1.value);
    }
    else if(operand1.subtype == sNil){
            strcat(_operand1,"nil@");
            strcat(_operand1,operand1.value);
    }*/
    instructionGenerator(L, Type, _operand1, "", "");
}

//instrukcia s 2 operandmi//////////////////////////////////////////////////////////

void instruction2op(tDLListInst *L, int Type, tOperand operand1, tOperand operand2) {
//operand 1
    char *_operand1 = malloc(sizeof(char) * 100);
//premenna identifikator
    if (operand1.type == sIdentificator) {
        if (strcmp(operand1.frame, "GF") == 0) {
            strcat(_operand1, "GF@");
            strcat(_operand1, operand1.value);
        } else if (strcmp(operand1.frame, "LF") == 0) {
            strcat(_operand1, "LF@");
            strcat(_operand1, operand1.value);
        } else if (strcmp(operand1.frame, "TF") == 0) {
            strcat(_operand1, "TF@");
            strcat(_operand1, operand1.value);
        }
        if (operand1.tmp == true) {
            strcat(_operand1, "$tmp");
        }
        if (operand1.Label == true) {
            strcat("$", _operand1);
        }
    }
//konstatna
    else if (operand1.type == sString) {
        strcat(_operand1, "string@");
        strcat(_operand1, operand1.value);
    } else if (operand1.type == sNumber) {
        if (operand1.subtype == sInteger) {
            strcat(_operand1, "int@");
            strcat(_operand1, operand1.value);
        } else if (operand1.subtype == sDoublePointNumber || operand1.subtype == sDoubleExponentNumber) {
            strcat(_operand1, "float@");
            strcat(_operand1, operand1.value);
        }
    }
    /* else if(operand1.subtype == sBool){
             strcat(_operand1,"bool@");
             strcat(_operand1,operand1.value);
     }
     else if(operand1.subtype == sNil){
             strcat(_operand1,"nil@");
             strcat(_operand1,operand1.value);
     }*/
//operand 2

    char *_operand2 = malloc(sizeof(char) * 100);
//premenna identifikator
    if (operand2.type == sIdentificator) {
        if (strcmp(operand2.frame, "GF") == 0) {
            strcat(_operand2, "GF@");
            strcat(_operand2, operand2.value);
        } else if (strcmp(operand2.frame, "LF") == 0) {
            strcat(_operand2, "LF@");
            strcat(_operand2, operand2.value);
        } else if (strcmp(operand2.frame, "TF") == 0) {
            strcat(_operand2, "TF@");
            strcat(_operand2, operand2.value);
        }
        if (operand2.tmp == true) {
            strcat(_operand2, "$tmp");
        }
        if (operand2.Label == true) {
            strcat("$", _operand2);
        }
    }
//konstatna
    else if (operand2.type == sString) {
        strcat(_operand2, "string@");
        strcat(_operand2, operand2.value);
    } else if (operand2.type == sNumber) {
        if (operand2.subtype == sInteger) {
            strcat(_operand2, "int@");
            strcat(_operand2, operand2.value);
        } else if (operand2.subtype == sDoublePointNumber || operand2.subtype == sDoubleExponentNumber) {
            strcat(_operand2, "float@");
            strcat(_operand2, operand1.value);
        }
    }
    /*else if(operand2.subtype == sBool){
            strcat(_operand2,"bool@");
            strcat(_operand2,operand2.value);
    }
    else if(operand2.subtype == sNil){
            strcat(_operand2,"nil@");
            strcat(_operand2,operand2.value);
    } */
    instructionGenerator(L, Type, _operand1, _operand2, "");
}

//instrukcia s 3 operandmi ///////////////////////////////////////////////////////////////
void instruction3op(tDLListInst *L, int Type, tOperand operand1, tOperand operand2, tOperand operand3) {
//operand 1
    char *_operand1 = malloc(sizeof(char) * 100);
//premenna identifikator
    if (operand1.type == sIdentificator) {
        if (strcmp(operand1.frame, "GF") == 0) {
            strcat(_operand1, "GF@");
            strcat(_operand1, operand1.value);
        } else if (strcmp(operand1.frame, "LF") == 0) {
            strcat(_operand1, "LF@");
            strcat(_operand1, operand1.value);
        } else if (strcmp(operand1.frame, "TF") == 0) {
            strcat(_operand1, "TF@");
            strcat(_operand1, operand1.value);
        }
        if (operand1.tmp == true) {
            strcat(_operand1, "$tmp");
        }
        if (operand1.Label == true) {
            strcat("$", _operand1);
        }
    }
//konstatna
    else if (operand1.type == sString) {
        strcat(_operand1, "string@");
        strcat(_operand1, operand1.value);
    } else if (operand1.type == sNumber) {
        if (operand1.subtype == sInteger) {
            strcat(_operand1, "int@");
            strcat(_operand1, operand1.value);
        } else if (operand1.subtype == sDoublePointNumber || operand1.subtype == sDoubleExponentNumber) {
            strcat(_operand1, "float@");
            strcat(_operand1, operand1.value);
        }
    }
    /*  else if(operand1.subtype == sBool){
              strcat(_operand1,"bool@");
              strcat(_operand1,operand1.value);
      }
      else if(operand1.subtype == sNil){
              strcat(_operand1,"nil@");
              strcat(_operand1,operand1.value);
      }*/
//operand 2

    char *_operand2 = malloc(sizeof(char) * 100);
//premenna identifikator
    if (operand2.type == sIdentificator) {
        if (strcmp(operand2.frame, "GF") == 0) {
            strcat(_operand2, "GF@");
            strcat(_operand2, operand2.value);
        } else if (strcmp(operand2.frame, "LF") == 0) {
            strcat(_operand2, "LF@");
            strcat(_operand2, operand2.value);
        } else if (strcmp(operand2.frame, "TF") == 0) {
            strcat(_operand2, "TF@");
            strcat(_operand2, operand2.value);
        }
        if (operand2.tmp == true) {
            strcat(_operand2, "$tmp");
        }
        if (operand2.Label == true) {
            strcat("$", _operand2);
        }
    }
//konstatna
    else if (operand2.type == sString) {
        strcat(_operand2, "string@");
        strcat(_operand2, operand2.value);
    } else if (operand2.type == sNumber) {
        if (operand2.subtype == sInteger) {
            strcat(_operand2, "int@");
            strcat(_operand2, operand2.value);
        } else if (operand2.subtype == sDoublePointNumber || operand2.subtype == sDoubleExponentNumber) {
            strcat(_operand2, "float@");
            strcat(_operand2, operand1.value);
        }
    }
    /* else if(operand2.subtype == sBool){
             strcat(_operand2,"bool@");
             strcat(_operand2,operand2.value);
     }
     else if(operand2.subtype == sNil){
             strcat(_operand2,"nil@");
             strcat(_operand2,operand2.value);
     } */

//operand 3
    char *_operand3 = malloc(sizeof(char) * 100);
//premenna identifikator
    if (operand3.type == sIdentificator) {
        if (strcmp(operand3.frame, "GF") == 0) {
            strcat(_operand3, "GF@");
            strcat(_operand3, operand3.value);
        } else if (strcmp(operand3.frame, "LF") == 0) {
            strcat(_operand3, "LF@");
            strcat(_operand3, operand3.value);
        } else if (strcmp(operand3.frame, "TF") == 0) {
            strcat(_operand3, "TF@");
            strcat(_operand3, operand3.value);
        }
        if (operand3.tmp == true) {
            strcat(_operand3, "$tmp");
        }
        if (operand3.Label == true) {
            strcat( _operand3,"$");
            strcat("$",_operand3);
        }
    }
//konstatna
    else if (operand3.type == sString) {
        strcat(_operand3, "string@");
        strcat(_operand3, operand3.value);
    } else if (operand3.type == sNumber) {
        if (operand3.subtype == sInteger) {
            strcat(_operand3, "int@");
            strcat(_operand3, operand3.value);
        } else if (operand3.subtype == sDoublePointNumber || operand3.subtype == sDoubleExponentNumber) {
            strcat(_operand3, "float@");
            strcat(_operand3, operand3.value);
        }
    }
    /* else if(operand3.subtype == sBool){
             strcat(_operand3,"bool@");
             strcat(_operand3,operand3.value);
     }
     else if(operand3.subtype == sNil){
             strcat(_operand3,"nil@");
             strcat(_operand3,operand3.value);
     } */

    instructionGenerator(L, Type, _operand1, _operand2, _operand3);
}

//vypis instrukcii /////////////////////////////////////////////////////////////////////////////////////////////////////
void instructionPrinter(tDLListInst *L) {
    printf(".IFJcode19\n");
    DLFirst(L);
    while (DLActive(L)) {
        DLCopy(L, &printinst);
        switch (printinst.Type) {
            case MOVE:
                printf("MOVE %s %s\n", printinst.o1, printinst.o2);
                break;
            case CREATEFRAME:
                printf("CREATEFRAME\n");
                break;
            case PUSHFRAME:
                printf("PUSHFRAME\n");
                break;
            case POPFRAME:
                printf("POPFRAME\n");
                break;
            case DEFVAR:
                printf("DEFVAR %s\n", printinst.o1);
                break;
            case CALL:
                printf("CALL %s\n", printinst.o1);
                break;
            case RETURN:
                printf("RETURN\n");
                break;
            case PUSHS:
                printf("PUSHS %s\n", printinst.o1);
                break;
            case POPS:
                printf("POPS %s\n", printinst.o1);
                break;
            case CLEARS:
                printf("CLEARS\n");
                break;
            case ADD:
                printf("ADD %s %s %s\n", printinst.o1, printinst.o2, printinst.o3);
                break;
            case SUB:
                printf("SUB %s %s %s\n", printinst.o1, printinst.o2, printinst.o3);
                break;
            case MUL:
                printf("MUL %s %s %s\n", printinst.o1, printinst.o2, printinst.o3);
                break;
            case DIV:
                printf("DIV %s %s %s\n", printinst.o1, printinst.o2, printinst.o3);
                break;
            case IDIV:
                printf("IDIV %s %s %s\n", printinst.o1, printinst.o2, printinst.o3);
                break;
            case ADDS:
                printf("ADDS\n");
                break;
            case SUBS:
                printf("SUBS\n");
                break;
            case MULS:
                printf("MULS\n");
                break;
            case DIVS:
                printf("DIVS\n");
                break;
            case IDIVS:
                printf("IDIVS\n");
                break;
            case LT:
                printf("LT %s %s %s\n", printinst.o1, printinst.o2, printinst.o3);
                break;
            case GT:
                printf("GT %s %s %s\n", printinst.o1, printinst.o2, printinst.o3);
                break;
            case EQ:
                printf("EQ %s %s %s\n", printinst.o1, printinst.o2, printinst.o3);
                break;
            case LTS:
                printf("LTS\n");
                break;
            case GTS:
                printf("GTS\n");
                break;
            case EQS:
                printf("EQS\n");
                break;
            case AND:
                printf("AND %s %s %s\n", printinst.o1, printinst.o2, printinst.o3);
                break;
            case OR:
                printf("OR %s %s %s\n", printinst.o1, printinst.o2, printinst.o3);
                break;
            case NOT:
                printf("NOT %s %s %s\n", printinst.o1, printinst.o2, printinst.o3);
                break;
            case ANDS:
                printf("ANDS\n");
                break;
            case ORS:
                printf("ORS\n");
                break;
            case NOTS:
                printf("NOTS \n");
                break;
            case INT2FLOAT:
                printf("INT2FLOAT %s %s\n", printinst.o1, printinst.o1);
                break;
            case FLOAT2INT:
                printf("FLOAT2INT %s %s\n", printinst.o1, printinst.o1);
                break;
            case INT2CHAR:
                printf("INT2CHAR %s %s\n", printinst.o1, printinst.o1);
                break;
            case STRI2INT:
                printf("STRI2INT %s %s %s\n", printinst.o1, printinst.o2, printinst.o3);
                break;
            case INT2FLOATS:
                printf("INT2FLOATS\n");
                break;
            case FLOAT2INTS:
                printf("FLOAT2INTS\n");
                break;
            case INT2CHARS:
                printf("INT2CHARS\n");
                break;
            case STRI2INTS:
                printf("STRI2INTS\n");
                break;
            case READ:
                printf("READ %s %s\n", printinst.o1, printinst.o2);
                break;
            case WRITE:
                printf("WRITE %s\n", printinst.o1);
                break;
            case CONCAT:
                printf("CONCAT %s %s %s\n", printinst.o1, printinst.o2, printinst.o3);
                break;
            case STRLEN:
                printf("STRLEN %s %s\n", printinst.o1, printinst.o2);
                break;
            case GETCHAR:
                printf("GETCHAR %s %s %s\n", printinst.o1, printinst.o2, printinst.o3);
                break;
            case SETCHAR:
                printf("SETCHAR %s %s %s\n", printinst.o1, printinst.o2, printinst.o3);
                break;
            case TYPE:
                printf("TYPE %s %s\n", printinst.o1, printinst.o2);
                break;
            case LABEL:
                printf("LABEL %s\n", printinst.o1);
                break;
            case JUMP:
                printf("JUMP %s\n", printinst.o1);
                break;
            case JUMPIFEQ:
                printf("JUMPIFEQ %s %s %s\n", printinst.o1, printinst.o2, printinst.o3);
                break;
            case JUMPIFNEQ:
                printf("JUMPIFNEQ %s %s %s\n", printinst.o1, printinst.o2, printinst.o3);
                break;
            case JUMPIFEQS:
                printf("JUMPIFEQS %s\n", printinst.o1);
                break;
            case JUMPIFNEQS:
                printf("JUMPIFNEQS %s\n", printinst.o1);
                break;
            case EXIT:
                printf("EXIT\n");
                break;
            case BREAK:
                printf("BREAK\n");
                break;
            case DPRINT:
                printf("DPRINT %s\n", printinst.o1);
                break;
        }
        DLSucc(L);
    }
}

void functionLen(){
    operand1 = initOperand(operand1, "length",true,false,sIdentificator,-1,"");
    instruction1op(&instList, LABEL, operand1);
    instruction0op(&instList, PUSHFRAME);
    operand1 = initOperand(operand1,"a",false,true,sNumber,sInteger,"GF");
    operand2 = initOperand(operand2, "s", false,false,sString,-1,"LF");
    instruction2op(&instList, STRLEN, operand1, operand2);
    instruction0op(&instList, RETURN);
}

void functionChr()
{
    operand1 = initOperand(operand1, "chr",true,false, sIdentificator,-1,"");
    instruction1op(&instList, LABEL, operand1);
    instruction0op(&instList, PUSHFRAME);
    operand1 = initOperand(operand1,"a",false,true,sNumber,sInteger,"GF");
    operand2 = initOperand(operand2, "i", false,false,sString,-1,"LF");
    instruction2op(&instList, INT2CHAR, operand1, operand2);
    instruction0op(&instList, RETURN);
}

void functionOrd(){
    operand1 = initOperand(operand1, "ord",true,false,sIdentificator,-1,"");
    instruction1op(&instList, LABEL, operand1);
    instruction0op(&instList, PUSHFRAME);
}
