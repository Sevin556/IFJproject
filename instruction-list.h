/*
* Predmet  :   IFJ / IAL
* Súbor    :   instruction-list.h
* Projekt  :   Implementácia prekladača imperatívneho jazyka IFJ19
* Tým č    :   127
* Varianta :   I
* Autoři   : xhalom00, Ivan Halomi
*            xhiner00, Martin Hiner
*            xsevci64, Adam Ševčík
*            xzakji02, Jiří Žák
*/
#ifndef IFJ_INSTRUCTION_LIST_H
#define IFJ_INSTRUCTION_LIST_H

#include <stdbool.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "scanner.h"

typedef struct instruction{
    int Type;
    char* o1;
    char* o2;
    char* o3;
} tInstr;

typedef struct tDLElem {
    tInstr instruction;
    struct tDLElem *lptr;
    struct tDLElem *rptr;
} *tDLElemPtrInstruction;

typedef struct {
    tDLElemPtrInstruction First;
    tDLElemPtrInstruction Act;
    tDLElemPtrInstruction Last;
} tDLListInst;

typedef struct operand{
    char* value; // a, 9, 1s5615c, Hello world
	bool tmp;
	bool Label;
    int type;  //sIdentifikator,SNumber,sString
	int subtype; //sDouble,sInteger
    char* frame; //GF,LF,TF
}tOperand;


void DLInitList (tDLListInst *);
void DLDisposeList (tDLListInst *);
int DLInsertFirst (tDLListInst *, tInstr);
int DLInsertLast(tDLListInst *, tInstr);
void DLFirst (tDLListInst *);
void DLLast (tDLListInst *);
void DLCopyFirst (tDLListInst *, tInstr *);
void DLCopyLast (tDLListInst *, tInstr *);
void DLDeleteFirst (tDLListInst *);
void DLDeleteLast (tDLListInst *);
void DLPostDelete (tDLListInst *);
void DLPreDelete (tDLListInst *);
int DLPostInsert (tDLListInst *, tInstr);
int DLPreInsert (tDLListInst *, tInstr);
int DLCopy (tDLListInst *, tInstr *);
void DLActualize (tDLListInst *, tInstr);
void DLSucc (tDLListInst *);
void DLPred (tDLListInst *);
int DLActive (tDLListInst *);

void instructionGenerator(tDLListInst *, int, char*, char*, char*);
void instructionPrinter(tDLListInst *);

tOperand initOperand(tOperand , char*, bool, bool, int, int, char*); //operand, hodnota, typ, subtyp, frame

void instruction0op(tDLListInst *, int);
void instruction1op(tDLListInst *, int, tOperand);
void instruction2op(tDLListInst *, int, tOperand, tOperand);
void instruction3op(tDLListInst *, int, tOperand, tOperand, tOperand);

void functionLen();
void functionChr();
void functionOrd();

typedef enum{
    MOVE,
	CREATEFRAME,
	PUSHFRAME,
	POPFRAME,
	DEFVAR,
	CALL,
	RETURN,
	PUSHS,
	POPS,
	CLEARS,
	ADD,
	SUB,
	MUL,
	DIV,
	IDIV,
	ADDS,
	SUBS,
	MULS,
	DIVS,
	IDIVS,
	LT,
	GT,
	EQ,
	LTS,
	GTS,
	EQS,
	AND,
	OR,
	NOT,
	ANDS,
	ORS,
	NOTS,
	INT2FLOAT,
	FLOAT2INT,
	INT2CHAR,
	STRI2INT,
	INT2FLOATS,
	FLOAT2INTS,
	INT2CHARS,
	STRI2INTS,
	READ,
	WRITE,
	CONCAT,
	STRLEN,
	GETCHAR,
	SETCHAR,
	TYPE,
	LABEL,
	JUMP,
	JUMPIFEQ,
	JUMPIFNEQ,
	JUMPIFEQS,
	JUMPIFNEQS,
	EXIT,
	BREAK,
	DPRINT,
} I_TYPE;

#endif //IFJ_INSTRUCTION_LIST_H