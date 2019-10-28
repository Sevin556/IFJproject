//
// Created by zak on 28.10.19.
//

#ifndef IFJ_STRING_H
#define IFJ_STRING_H

#include <stdlib.h>
#include "err_code.h"
#include <string.h>

#define INIT_ALLOC_SIZE 10

typedef struct {
    char *value; // retezec
    int length; // delka retezce
    int lengthAllocated; // alokovana delka
} string;

int stringInit(string *str);
int stringAddChar(string *str, char c);
int stringAddString(string *str, char *c);

#endif //IFJ_STRING_H
