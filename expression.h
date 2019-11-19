//
// Created by zak on 18.11.19.
//

#ifndef IFJ_EXPRESSION_H
#define IFJ_EXPRESSION_H

#include "err_code.h"
#include "string.h"
#include "stack.h"
#include "scanner.h"
#include "symtable.h"
#include <string.h>

typedef enum {
    ePlus,          // +
    eMinus,         // -
    eEqual,         // ==
    eNotEqual,      // !=
    eLess,          // <
    eLessEqual,     // <=
    eMore,          // >
    eMoreEqual,     // >=
    eOther

} exprItem;

#endif //IFJ_EXPRESSION_H
