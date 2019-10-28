//
// Created by zak on 28.10.19.
//
#include "string.h"

int stringInit(string *str){
    str->value = (char*)malloc(sizeof(char)*INIT_ALLOC_SIZE);
    if(str->value == NULL)
        return ERR_INTERN;
    str->length = 0;
    str->value = '\0';
    str->lengthAllocated = INIT_ALLOC_SIZE;
    return OK;
}

int stringAddChar(string *str, char c){
    if(str->length + 1 >= str->lengthAllocated){
        str->value = (char*)realloc(str->value, str->lengthAllocated + INIT_ALLOC_SIZE +sizeof(char));
        if(str->value == NULL)
            return ERR_INTERN;
        str->lengthAllocated += INIT_ALLOC_SIZE;
    }
    str->value[str->length] = c;
    str->length++;
    str->value[str->length] = '\0';
    return OK;
}

int stringAddString(string *str, char *c){
    int result_code;
    for (unsigned int i = 0; i < strlen(c); i++) {
        result_code = stringAddChar(str, c[i]);
    }
    return result_code;
}
