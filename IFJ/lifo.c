
#include "lifo.h"


unsigned STACK_SIZE = 1000;

void stackInit ( tStack* s ) {
	if(s == NULL)
		return;

	s->top = -1;
	s->arr = (tRedukToken*) malloc(STACK_SIZE * sizeof(tRedukToken));
    if (s->arr == NULL)
    {
        fprintf(stderr,"Nepodarilo sa alokovať pamäť");
        exit(ERR_INTERN);
    }
	s->size = STACK_SIZE;
}

void stackRealloc ( tStack* s ){
	tRedukToken *new = (tRedukToken*)realloc(s->arr, (s->size + STACK_SIZE)*sizeof(tRedukToken));
	{
        fprintf(stderr,"Nepodarilo sa alokovať viac pamäte");
        exit(ERR_INTERN);
    }
	s->arr = new;
	s->size += STACK_SIZE;
}

void stackFree(tStack *s)
{
    if (s != NULL)
    {
        free(s->arr);
    }
}

int stackEmpty (tStack* s ) {
	return ((s->top < 0) ? 1 : 0);
}

int stackFull (tStack* s ) {
	return ((s->top < (int)s->size) ? 0 : 1);
}
int IndexOfTop(tStack* s)
{
	return s->top;
}

tRedukToken *stackTop ( tStack* s) {
	if (stackEmpty(s))
		return NULL;
	else
		return (&(s->arr[s->top]));
}
tRedukToken *stackIndex ( tStack* s,int index) {
	if (stackEmpty(s))
		return NULL;
	else
		return (&(s->arr[index]));
}

void stackPop(tStack* s)
{
	s->top--;
}


tRedukToken *stackTopPop ( tStack* s) {
	tRedukToken *temp = stackTop(s);
	stackPop(s);
	return temp;
}
void stackPush ( tStack* s, tRedukToken *token ) {
	if(stackFull(s))
    {
		stackRealloc(s);
    }
    s->arr[++(s->top)] = *token;
}
