#include <stdbool.h>

void DLInitList (tDLListInst *);
void DLDisposeList (tDLListInst *);
void DLInsertFirst (tDLListInst *, tInstr);
void DLInsertLast(tDLListInst *, tInstr);
void DLFirst (tDLListInst *);
void DLLast (tDLListInst *);
void DLCopyFirst (tDLListInst *, tInstr *);
void DLCopyLast (tDLListInst *, tInstr *);
void DLDeleteFirst (tDLListInst *);
void DLDeleteLast (tDLListInst *);
void DLPostDelete (tDLListInst *);
void DLPreDelete (tDLListInst *);
void DLPostInsert (tDLListInst *, tInstr);
void DLPreInsert (tDLListInst *, tInstr);
void DLCopy (tDLListInst *, tInstr *);
void DLActualize (tDLListInst *, tInstr);
void DLSucc (tDLListInst *);
void DLPred (tDLListInst *);
int DLActive (tDLListInst *);

void instructionGenerator(tDLListInst *, int, void *, void *, void *);

tOperand initOperand(tOperand , char*, char*, char*); //operand, hodnota, typ, frame

void instruction0op(tDLListInst *, int);
void instruction1op(tDLListInst *, int, tOperand);
void isntruction2op(tDLListInst *, int, tOperand, tOperand);
void instruction3op(tDLListInst *, int, tOperand, tOperand, tOperand);


typedef struct {
    int Type;
    void * a1;
    void * a2;
    void * a3;
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
    string value;
    int type;
    string frame;
}tOperand;

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
	ADDS,
	SUBS,
	MULS,
	DIVS,
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
	FLOAT2R2EINT,
	FLOAT2R20INT,
	INT2CHAR,
	STRI2INT,
	INT2FLOATS,
	FLOAT2INTS,
	FLOAT2R2EINTS,
	FLOAT2R20INTS,
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
	BREAK,
	DPRINT,
} INSTRUCTION;