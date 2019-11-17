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

tSymtable gTable; // globalna tabulka 
tSymtable lTable; // lokalna tabulka



void printTest(){
    printf("test\n");
}

/*int tokenType(){
}*/
ERR_VAL doParse(){      //toto budes rekurzivne volat 

}


//zavolas len raz niekde v maine 
ERR_VAL parse(){    

symTableInit(&gTable);

ERR_VAL rett;
if ((rett=doParse) != OK)
{
	printf("failure ");

}

}
