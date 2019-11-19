//
// Created by zak on 19.11.19.
//

#include "expression.h"

ERR_VAL expression (tToken first){
    ERR_VAL rett;
    ptrStack expr_stack;
    expressionInit(&expr_stack);

    SDispose(&expr_stack);
    return rett;
}

ERR_VAL expressionInit (ptrStack *expr_stack){
    if(expr_stack == NULL){
        SInit(expr_stack);
        return OK;
    }
    return ERR_SYN;
}