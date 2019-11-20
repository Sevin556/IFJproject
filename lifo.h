#ifndef _LIFO_H_
#define _LIFO_H_

#include "scanner.h"
#include <stdio.h>
#include "exprParser.h"
#include <stdbool.h>
extern unsigned STACK_SIZE;

/* ADT zásobník implementovaný ve statickém poli */
typedef struct {
	unsigned size;
	tRedukToken *arr; /* pole pro uložení hodnot */
	int top; /* index prvku na vrcholu zásobníku */
} tStack;

/**
 * @brief Provede inicializaci zásobníku - nastaví vrchol zásobníku.
 */
void stackInit(tStack* s);

/**
 * Vrací nenulovou hodnotu, pokud je zásobník prázdný, jinak vrací hodnotu 0.
 */
int stackEmpty(tStack* s);

/**
 * @brief Vrací nenulovou hodnotu, je-li zásobník plný, jinak vrací hodnotu 0.
 */
int stackFull(tStack* s);

/**
 * @brief Vrací znak z vrcholu zásobníku.
 */
tRedukToken *stackTop(tStack* s);
/**
 *Vrací velikost zásobníku.
 */
int stackLenght(tStack *s);
/**
 * @brief Odstraní prvek z vrcholu zásobníku
 */
void stackPop( tStack* s);

tRedukToken *stackTopPop ( tStack* s);
/**
 * @brief Vloží znak na vrchol zásobníku.
 */
bool checkMoreOperators(tStack* s);
void stackPush(tStack* s, tRedukToken token);
void stackPrint(char* stackName, tStack* s);
void stackClear(tStack* s);
#endif