/*
* Predmet  :   IFJ / IAL
* Súbor    :   string.c - Pomocna knihovna lexikálního analyzátoru
* Projekt  :   Implementácia prekladača imperatívneho jazyka IFJ19
* Tým č    :   127
* Varianta :   I
* Autoři   : xhalom00, Ivan Halomi
*            xhiner00, Martin Hiner
*            xsevci64, Adam Ševčík
*            xzakji02, Jiří Žák
*/

#include "string.h"

int stringInit(string *str) {
    str->value = malloc(sizeof(char) * INIT_ALLOC_SIZE);
    if (str->value == NULL)
        return ERR_INTERN;
    str->length = 0;
    str->value[0] = '\0';
    str->lengthAllocated = INIT_ALLOC_SIZE;
    return OK;
}

int stringAddChar(string *str, char c) {
    if (str->length + 1 >= str->lengthAllocated) {
        str->value = realloc(str->value, str->lengthAllocated + INIT_ALLOC_SIZE + sizeof(char));
        if (str->value == NULL)
            return ERR_INTERN;
        str->lengthAllocated += INIT_ALLOC_SIZE;
    }
    str->value[str->length] = c;
    str->length++;
    str->value[str->length] = '\0';
    return OK;
}

int stringAddString(string *str, char *c) {
    int result_code;
    for (unsigned int i = 0; i < strlen(c); i++) {
        result_code = stringAddChar(str, c[i]);
    }
    return result_code;
}

bool stringCompare(string *a, string *b){
    if (strcmp(a->value, b->value) == 0)
        return true;
    return false;
}

void stringChangeLastChar(string *str, char c) {
    if (str->length > 0) {
        str->value[str->length - 1] = c;
    }
}

void stringDeleteLastChar(string *str) {
    str->value[str->length - 1] = '\0';
    str->length--;
}

void stringFree(string *str) {
    free(str->value);
    str->value = NULL;
    str->length = str->lengthAllocated = 0;
}
