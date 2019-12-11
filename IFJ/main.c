/*
* Predmet  :   IFJ / IAL
* Súbor    :   main.c
* Projekt  :   Implementácia prekladača imperatívneho jazyka IFJ19
* Tým č    :   127
* Varianta :   I
* Autoři   : xhalom00, Ivan Halomi
*            xhiner00, Martin Hiner
*            xsevci64, Adam Ševčík
*            xzakji02, Jiří Žák
*/
#include <stdio.h>
#include "parser.h"
#include "err_code.h"
#include "instruction-list.h"

tSymtable gTable;               // globalna tabulka
tSymtable lTable;               // lokalni tabulka
tDLListInst instList;
tDLListInst funcList;

int main(int argc, char **argv) {
    (void)argv;
    if(argc > 1) {
        fprintf(stderr, "Program nesmi být volán s parametry");
        return ERR_INTERN;
    }

    int rett = OK;
    symTableInit(&gTable);
    symTableInit(&lTable);
    DLInitList(&instList,0);
    DLInitList(&funcList,1);
    symTableInsertVesFunction(&gTable);

    rett = parse();

    printf("#%d\n", rett);
    if(rett == OK){
        instructionPrinter(&funcList,1);
        instructionPrinter(&instList,0);
    }
        

    DLDisposeList(&instList);
    symTableDispose(&gTable);
    symTableDispose(&lTable);

    return rett;
}