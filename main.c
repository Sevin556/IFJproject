#include <stdio.h>
#include "parser.h"
#include "err_code.h"
#include "scanner.h"
#include "err_code.h"
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include "string.h"

int main(int argc, char *argv[])
{
    tToken Jozovkokot;
   while(Jozovkokot->type != sEOF){
       Jozovkokot = get_token(void);
       printf("%d",Jozovkokot->type);
   }

   return 0;
}
