#include "scanner.h"
#include "exprParser.h"
#include "instruction-list.h"
#include "err_code.h"
bool inMain = true;


int main() {

    //for (int i = 0;i<20; i++)
    tToken* hej;
    hej = get_token();
    while (hej->type != 2)

    
    {
         
    //hej = get_token();
    int vysledok = 0;
    vysledok = exprParsing(hej);
    printf("****************************************************************************************\n");
    printf("MAIN vysledok je %d ",vysledok);
    hej = get_token();

    printf("\n MAIN dalsi token je %d \n",hej->type);
    }
   
    return OK;
}

