#include "string.h"
#include "scanner.h"
#include <stdio.h>

int main() {
        tToken *token = init_token();       
        int i = 0;

        while(token->type != sEOF) {
                token = get_token();
                i++;

                if(token->subtype == -1) {
                        printf("%d. \"%s\"%-10s\ttype: %-7d\t\t\tline: %-4d\n", i, token->data.value, "", token->type, token->line);
                } else {
                        printf("%d. \"%s\"%-10s\ttype: %-3d\tsubtype: %-3d\tline: %-4d\n", i, token->data.value, "", token->type, token->subtype, token->line);
                }
        }
        printf("\n\tAll OK\n");
        return 0;
}
