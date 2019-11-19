/*
 * Predmet  :   IFJ / IAL
 * Súbor    :   scanner.h - Hlavičkový súbor lexikálneho analyzátora
 * Projekt  :   Implementácia prekladača imperatívneho jazyka IFJ19
 * Tým č    :   127
 * Varianta :   I
 * Autoři   : xhalom00, Ivan Halomi
 *            xhiner00, Martin Hiner
 *            xsevci64, Adam Ševčík
 *            xzakji02, Jiří Žák
 */

#include "scanner.h"
#include "err_code.h"
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

tToken* get_token(void) {
        tState state;
        bool FirstToken;
        char c;
        tToken *token = init_token();

        static int indent_cnt = 0;
        static int line_cnt = 0;

        state = sStart;       /* počiatocný stav */

        while(true) {
                c = getchar();

                switch(state) {
                case sStart:
                        /****************počiatocný stav********************/
                        /* End Of File */
                        if(c == EOF) {
                                stringAddString(&token->data, "EOF");
                                token->line = line_cnt;
                                token->type = sEOF;

                                return token;
                        } else

                        /* End Of Line */
                        if(c == '\n') {
                                if(FirstToken) {
                                        line_cnt++;
                                        state = sStart;
                                } else {
                                        FirstToken = true;
                                        line_cnt++;

                                        stringAddChar(&token->data, '\n');
                                        token->line = line_cnt;
                                        token->type = sEOL;
                                }
                        } else

                        /* space */
                        if(isspace(c)) {
                                if(FirstToken) {
                                        state = sDentDecide;
                                        indent_cnt++;         /* zvýšenie počítadla indentu */
                                        FirstToken = false;
                                } else {
                                        state = sStart;
                                }
                        } else

                        /* line comment */
                        if(c == '#') {
                                state = sLineComment;
                        } else

                        /* block comment */
                        if(c == '"') {
                                token->line = line_cnt;
                                /* 1st " */
                                c = getchar();
                                if(c == '"') {
                                        /* 2nd " */
                                        c = getchar();
                                        if(c == '"') {
                                                /* 3rd " - OK */

                                                state = sBlockComment;
                                        } else {
                                                state = sLexError;
                                        }
                                } else {
                                        state = sLexError;
                                }
                        } else

                        /* string */
                        if(c == '\'') {
                                token->line = line_cnt;

                                state = sStringStart;
                        }

                        /* id/key */
                        if(isalpha(c)) {
                                token->line = line_cnt;
                                stringAddChar(&token->data, c);

                                state = sIdentificatorOrKeyword;
                        } else
                        if(c == '_') {
                                token->line = line_cnt;
                                stringAddChar(&token->data, c);

                                state = sIdentificator;
                        } else

                        /* number */
                        if(isdigit(c)) {
                                token->line = line_cnt;
                                stringAddChar(&token->data, c);

                                state = sIntegerOrDouble;
                        }

                        break;
                /**************koniec počiatočného stavu*************/


                //TODO
                /************** EOL Start ****************/
                case sEOL:

                        break;
                /************** EOL End ****************/

                /************** Line Comment Start ****************/
                case sLineComment:

                        if((c == '\n') || (c == EOF)) {
                                state = sStart;
                        } else {
                                state = sLineComment;
                        }

                        break;
                /************** Line Comment End ****************/

                /************** Block Comment Start ****************/
                case sBlockComment:

                        if(c == '"') {
                                /* 1st " */
                                c = getchar();
                                if(c == '"') {
                                        /* 2nd " */
                                        c = getchar();
                                        if(c == '"') {
                                                /* 3rd " - OK */

                                                state = sStart;
                                        } else {
                                                state = sBlockComment;
                                        }
                                } else {
                                        state = sBlockComment;
                                }
                        } else {
                                state = sBlockComment;
                        }

                        break;
                /************** Block Comment End ****************/


                /********* String Start**************************************/
                /************** String Start Start (lol)****************/
                case sStringStart:

                        if(c == '\'') {
                                /* prázdny string */
                                stringAddString(&token->data, "");
                                token->type = sString;
                        } else
                        if(c == '\\') {
                                /* escape sequence */
                                state = sStringEscape;
                        } else
                        if(c > 31) {
                                /* literál */
                                stringAddChar(&token->data, c);
                                state = sStringRead;
                        } else {
                                /* error */
                                state = sLexError;
                        }

                        break;
                /************** String Start End (lol) ****************/

                /************** String Escape Start ****************/
                case sStringEscape:

                        switch(c) {
                        case 'n':
                                stringAddChar(&token->data, '\n');
                                state = sStringRead;
                                break;

                        case 't':
                                stringAddChar(&token->data, '\t');
                                state = sStringRead;
                                break;

                        case '\'':
                                stringAddChar(&token->data, '\'');
                                state = sStringRead;
                                break;

                        case '\"':
                                stringAddChar(&token->data, '\"');
                                state = sStringRead;
                                break;

                        case '\\':
                                stringAddChar(&token->data, '\\');
                                state = sStringRead;
                                break;

                        case 'x':
                                state = sStringEscapeHex;
                                break;

                        default:
                                stringAddChar(&token->data, '\\');
                                stringAddChar(&token->data, c);
                                state = sStringRead;
                        }

                        break;
                /************** String Escape End ****************/

                /************** String Escape Hex Start ****************/
                case sStringEscapeHex:

                        if((isdigit(c)) || ((c >= 65) && (c <= 70)) || ((c >= 97) && (c <= 102))) {
                                /*   0-9                   A-F                         a-f   */
                                char temp = c;
                                c = getchar();

                                if((isdigit(c)) || ((c >= 65) && (c <= 70)) || ((c >= 97) && (c <= 102))) {
                                        /* escape success - return xx */
                                        stringAddChar(&token->data, temp);
                                        stringAddChar(&token->data, c);
                                        state = sStringRead;
                                } else {
                                        /* escape failed - return \xx */
                                        stringAddChar(&token->data, '\\');
                                        stringAddChar(&token->data, temp);
                                        stringAddChar(&token->data, c);
                                        state = sStringRead;
                                }

                        } else {
                                /* escape failde return \x */
                                stringAddChar(&token->data, '\\');
                                stringAddChar(&token->data, 'x');
                                stringAddChar(&token->data, c);
                                state = sStringRead;
                        }

                        break;
                /************** String Escape Hex End ****************/

                /************** String Read Start ****************/
                case sStringRead:

                        if(c == '\'') {
                                /* koniec stringu */

                                return token;
                        } else
                        if(c == '\\') {
                                /* escape sequence */
                                state = sStringEscape;
                        } else
                        if(c > 31) {
                                /* literál */
                                stringAddChar(&token->data, c);
                                state = sStringRead;
                        } else {
                                /* error */
                                state = sLexError;
                        }

                        break;
                /************** String Read End ****************/
                /*********String End *****************************/


                /********* Keyword / ID Start ******************/
                /************** ID/Key Start ****************/
                case sIdentificatorOrKeyword:
                        if(isalpha(c)) {
                                stringAddChar(&token->data, c);
                                state = sIdentificatorOrKeyword;
                        } else
                        if(isdigit(c) || c == '_') {
                                stringAddChar(&token->data, c);
                                state = sIdentificator;
                        } else {
                                /* koniec id/key - return */
                                ungetc(c, stdin);

                                assignType(token);
                                return token;
                        }
                        assignType(token);
                        break;
                /************** ID/Key End ****************/

                /************** ID Start ****************/
                case sIdentificator:
                        if(isalnum(c) || c == '_') {
                                stringAddChar(&token->data, c);
                                state = sIdentificator;
                        } else {
                                /* koniec id - return */
                                ungetc(c, stdin);

                                token->type = sIdentificator;
                                return token;
                        }
                        break;
                /************** ID End ****************/

                /********* Keyword / ID End ******************/

                /********* Number Start**************************************/
                /************** Int/Double Start ****************/
                case sIntegerOrDouble:
                        if(isdigit(c)) {
                                stringAddChar(&token->data, c);

                                state = sIntegerOrDouble;
                        } else
                        if(c == '.') {
                                stringAddChar(&token->data, c);

                                state = sDoublePoint;
                        } else
                        if((c == 'E') || (c == 'e')) {
                                stringAddChar(&token->data, c);

                                state = sDoubleExponent;
                        } else {
                                /* koniec number - return integer */
                                ungetc(c, stdin);

                                token->type = sInteger;
                                return token;
                        }
                        break;
                /************** Int/Double End ****************/

                /************** Double Point Start ****************/
                case sDoublePoint:
                        if(isdigit(c)) {
                                stringAddChar(&token->data, c);

                                state = sDoublePointNumber;
                        } else {
                                state = sLexError;
                        }
                        break;
                /************** Double Point End ****************/

                /************** EOL Start ****************/
                case sDoubleExponentNumber:
                        if(isdigit(c)) {

                        }
                        break;
                        /************** EOL End ****************/

                        /********* Number End ******************/

                }
        }
}

/* inicializácia tokenu */
tToken* init_token(void) {
  tToken* tmp = (tToken*) malloc(sizeof(struct structToken));

  if(tmp == NULL) {
          fprintf(stderr, "Chyba %d - chyba alokácie pamäte\n", ERR_INTERN);
          exit(ERR_INTERN);
  }

  tmp->line = -1;
  tmp->type = -1;
  tmp->subtype = -1;
  stringInit(&tmp->data);

  return tmp;
}


/* funkcia zistí typ tokenu podľa obsahu data */
void assignType(tToken* t) {
        if(strcmp(t->data.value, "def") == 0) {
                t->type = sDef;
        } else
        if(strcmp(t->data.value, "else") == 0) {
                t->type = sElse;
        } else
        if(strcmp(t->data.value, "if") == 0) {
                t->type = sIf;
        } else
        if(strcmp(t->data.value, "None") == 0) {
                t->type = sNone;
        } else
        if(strcmp(t->data.value, "pass") == 0) {
                t->type = sPass;
        } else
        if(strcmp(t->data.value, "return") == 0) {
                t->type = sReturn;
        } else
        if(strcmp(t->data.value, "while") == 0) {
                t->type = sWhile;
        } else {
                /* nieje keyword */
                t->type = sIdentificator;
        }
}
