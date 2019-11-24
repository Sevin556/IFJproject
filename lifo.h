#ifndef _LIFO_H_
#define _LIFO_H_

//#include "scanner.h"
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
 *Vracia velkosť stacku
 */
int stackLenght(tStack *s);
/**
 * @brief Popne vrchny prvok
 */
void stackPop( tStack* s);

tRedukToken *stackTopPop ( tStack* s);
/**
 * @brief Vrati prvok z daneho indexu
 */
tRedukToken *stackIndex ( tStack* s,int index);

/**
 * @brief Pushne prvok na top stacku
 */
void stackPush(tStack* s, tRedukToken token);

/**
 * @brief Vrati index topu
 */
int IndexOfTop(tStack* s);
#endif