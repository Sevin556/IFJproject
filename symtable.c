
#include "symtable.h"

void BSTInit(tBSTNodePtr *root) {
    (*root) = NULL;
}

tBSTNodePtr BSTSearch(tBSTNodePtr root, char *K) {
    if (root != NULL) {
        if (strcmp(K, root->Key) < 0) {
            return BSTSearch(root->LPtr, K);
        } else if (strcmp(K, root->Key) > 0) {
            return BSTSearch(root->RPtr, K);
        } else {
            return root;
        }
    } else {
        return NULL;
    }
}

void BSTInsert(tBSTNodePtr *root, char *K, void *data, tNodeDataType nodeType) {
    if (*root == NULL) {
        tBSTNodePtr new = malloc(sizeof(struct tBSTNode));
        if (new == NULL)
            return;
        new->RPtr = NULL;
        new->LPtr = NULL;
        new->Data = data;
        new->Key = K;
        new->nodeDataType = nodeType;
    } else if ((*root)->Key == K) {
        (*root)->Data = data;
    } else if ((*root)->Key < K) {
        BSTInsert(&(*root)->RPtr, K, data, nodeType);
    } else {
        BSTInsert(&(*root)->LPtr, K, data, nodeType);
    }
}

void ReplaceByRightmost(tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr) {
    if (*RootPtr == NULL) {
        return;
    } else if ((*RootPtr)->RPtr != NULL) {
        ReplaceByRightmost(PtrReplaced, &(*RootPtr)->RPtr);
    } else {
        PtrReplaced->Key = (*RootPtr)->Key;
        PtrReplaced->Data = (*RootPtr)->Data;
        PtrReplaced->nodeDataType = (*RootPtr)->nodeDataType;
        BSTDelete(RootPtr, (*RootPtr)->Key);
    }
}

void BSTDelete(tBSTNodePtr *root, char *K) {
    tBSTNodePtr previous = NULL;

    if (*root == NULL) {
        return;
    } else if ((*root)->Key == K) {
        if ((*root)->RPtr == NULL && (*root)->LPtr != NULL) {
            previous = *root;
            *root = (*root)->LPtr;
            free(previous);
        } else if ((*root)->RPtr != NULL && (*root)->LPtr == NULL) {
            previous = *root;
            *root = (*root)->RPtr;
            free(previous);
        } else if ((*root)->RPtr != NULL && (*root)->LPtr != NULL) {
            ReplaceByRightmost(*root, &(*root)->LPtr);
        } else {
            previous = *root;
            *root = NULL;
            free(previous);
        }
    } else {
        BSTDelete(&(*root)->RPtr, K);
        BSTDelete(&(*root)->LPtr, K);
    }
}

void BSTDispose(tBSTNodePtr *root) {
    if (*root != NULL) {
        BSTDispose(&(*root)->LPtr);
        BSTDispose(&(*root)->RPtr);

        free((*root)->Key);
        free((*root)->Data);
        free(*root);
        *root = NULL;
    }
}


void symTableInit(tSymtable *Tab) {
    BSTInit(&(Tab->root));
}

void symTableInsertVariable(tSymtable *Tab, string str) {
    tVariable *data = malloc(sizeof(struct variable));
    if (data == NULL)
        return;
    data->retType = -1;
    BSTInsert(&(Tab->root), str.value, data, ndtVariable);
}

void symTableInsertFunction(tSymtable *Tab, string str) {
    tFunction *data = malloc(sizeof(struct function));
    if (data == NULL)
        return;
    string parametrs;
    stringInit(&parametrs);
    data->retType = -1;
    data->declared = data->defined = NULL;
    data->param = parametrs;
    BSTInsert(&(Tab->root), str.value, data, ndtFunction);
}

tBSTNodePtr symTableSearch(tSymtable *Tab, string str) {
    return BSTSearch(Tab->root, str.value);
}

void symTableDelete(tSymtable *Tab, string str) {
    BSTDelete(&(Tab->root), str.value);
}

void symTableDispose(tSymtable *Tab) {
    BSTDispose(&(Tab->root));
}

/*void symTableInsertVesFunction(tSymtable *Tab){
    tBSTNodePtr node;
    tFunction *fun;

    //len
    string len;
    stringInit(&len);
    stringAddString(&len, "len");
    symTableInsertFunction(Tab, len);
    node = symTableSearch(Tab, len);
    fun = (tFunction *)(node->Data);
    fun->declared = fun->defined = true;
    stringAddChar(&(fun->param), 's');
    stringInit(&(fun->paramName[0]));
    stringAddChar(&(fun->paramName[0]), 's');
    //fun->retType = sInteger;

    //substr
    string substr;
    stringInit(&substr);
    stringAddString(&substr, "substr");
    symTableInsertFunction(Tab, substr);
    node = symTableSearch(Tab, substr);
    fun = (tFunction *)(node->Data);
    fun->defined = fun->declared = true;
    stringAddString(&(fun->param), "sii");
    stringInit(&(fun->paramName[0]));
    stringAddChar(&(fun->paramName[0]), 's');
    stringInit(&(fun->paramName[1]));
    stringAddChar(&(fun->paramName[1]), 'i');
    stringInit(&(fun->paramName[2]));
    stringAddChar(&(fun->paramName[2]), 'n');
    //fun->retType = sString;

    //ord
    string ord;
    stringInit(&ord);
    stringAddString(&ord, "ord");
    symTableInsertFunction(Tab, ord);
    node = symTableSearch(Tab, ord);
    fun = (tFunction *)(node->Data);

}*/
