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
        bool FirstToken = true;
        char c;
        static tStack stack[MAX_DENT];
        stack[0] = 0;      /* zarážka */
        tToken *token = init_token();

        int indent_cnt = 0;
        static int line_cnt = 0;

        state = sStart;       /* počiatocný stav */

        while(true) {
                c = getchar();

                switch(state) {
                case sStart:
                        /****************počiatocný stav********************/
                        /* check Dedent */
                        if(FirstToken && c != ' ') {
                                ungetc(c, stdin);
                                if(stackEmpty(stack)) {
                                        /* no indentation */
                                        FirstToken = false;
                                } else {
                                        /* dedent needed */
                                        stackPop(stack);
                                        state = sDedent;
                                }
                        }

                        /* End Of File */
                        if(c == EOF) {
                                state = sEOF;
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
                                        indent_cnt++;         /* zvýšenie počítadla indentu */
                                        state = sDentDecide;
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
                        } else

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
                        } else

                        /* operators */
                        if(c == '+') {
                          token->line = line_cnt;
                          stringAddChar(&token->data, c);

                          state = sPlus;
                        } else
                        if(c == '-') {
                          token->line = line_cnt;
                          stringAddChar(&token->data, c);

                          state = sMinus;
                        } else
                        if(c == '*') {
                          token->line = line_cnt;
                          stringAddChar(&token->data, c);

                          state = sMultiplication;
                        } else
                        if(c == '/') {
                          token->line = line_cnt;
                          stringAddChar(&token->data, c);

                          state = sDivideFloat;
                        } else
                        if(c == '>') {
                          token->line = line_cnt;
                          stringAddChar(&token->data, c);

                          state = sMore;
                        } else
                        if(c == '<') {
                          token->line = line_cnt;
                          stringAddChar(&token->data, c);

                          state = sLess;
                        } else
                        if(c == '!') {
                          token->line = line_cnt;
                          stringAddChar(&token->data, c);

                          state = sInequal;
                        } else
                        if(c == '=') {
                          token->line = line_cnt;
                          stringAddChar(&token->data, c);

                          state = sAssignment;
                        } else
                        if(c == '(') {
                          token->line = line_cnt;
                          stringAddChar(&token->data, c);

                          state = sLeftBracket;
                        } else
                        if(c == ')') {
                          token->line = line_cnt;
                          stringAddChar(&token->data, c);

                          state = sRightBracket;
                        } else
                        if(c == ',') {
                          token->line = line_cnt;
                          stringAddChar(&token->data, c);

                          state = sComma;
                        }

                        break;
                /**************koniec počiatočného stavu*************/

                /************** sEOF Start ****************/
                case sEOF:
                        stringAddString(&token->data, "EOF");
                        token->line = line_cnt;
                        token->type = sEOF;

                        return token;

                        break;
                /************** sEOF End ****************/

                //TODO
                /************** sEOL Start ****************/
                case sEOL:

                        break;
                /************** sEOL End ****************/

                /********* Dent Start**************************************/
                /************** sDentDecide Start ****************/
                case sDentDecide:
                        if(isspace(c)) {
                                indent_cnt++;
                                state = sDentDecide;
                        } else
                        if(c == '\n') {
                                state = sStart;
                        } else {
                                int curr = stackTop(stack);
                                if(indent_cnt > curr) {
                                        //TODO
                                }
                        }
                        break;
                /************** sDentDecide End ****************/

                /************** sIndent Start ****************/
                case sIndent:
                        token->type = sIndent;
                        return token;
                        break;
                /************** sIndent End ****************/

                /************** sDedent Start ****************/
                case sDedent:
                        token->type = sDedent;
                        return token;
                        break;
                /************** sDedent End ****************/
                /********* Dent End**************************************/

                /************** sLineComment Start ****************/
                case sLineComment:

                        if((c == '\n') || (c == EOF)) {
                                state = sStart;
                        } else {
                                state = sLineComment;
                        }

                        break;
                /************** sLineComment End ****************/

                /************** sBlockComment Start ****************/
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
                /************** sBlockComment End ****************/


                /********* String Start**************************************/
                /************** sStringStart Start (lol)****************/
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
                /************** sStringStart End (lol) ****************/

                /************** sStringEscape Start ****************/
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
                /************** sStringEscape End ****************/

                /************** sStringEscapeHex Start ****************/
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
                /************** sStringEscapeHex End ****************/

                /************** sStringRead Start ****************/
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
                /************** sStringRead End ****************/
                /*********String End *****************************/


                /********* Keyword / ID Start ******************/
                /************** sIdentificatorOrKeyword Start ****************/
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
                /************** sIdentificatorOrKeyword End ****************/

                /************** sIdentificator Start ****************/
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
                /************** sIdentificator End ****************/

                /********* Keyword / ID End ******************/

                /********* Number Start**************************************/
                /************** sIntegerOrDouble Start ****************/
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

                                token->type = sNumber;
                                token->subtype = sInteger;
                                return token;
                        }
                        break;
                /************** sIntegerOrDouble End ****************/

                /************** sDoublePoint Start ****************/
                case sDoublePoint:
                        if(isdigit(c)) {
                                stringAddChar(&token->data, c);

                                state = sDoublePointNumber;
                        } else {
                                state = sLexError;
                        }
                        break;
                /************** sDoublePoint End ****************/

                /************** sDoubleExponent Start ****************/
                case sDoubleExponent:
                        if(isdigit(c)) {
                                stringAddChar(&token->data, c);

                                state = sDoubleExponentNumber;
                        } else
                        if((c == '+') || (c == '-')) {
                                stringAddChar(&token->data, c);

                                state = sDoubleExponentOperator;
                        }
                        break;
                /************** sDoubleExponent End ****************/

                /************** sDoubleExponentOperator Start ****************/
                case sDoubleExponentOperator:
                        if(isdigit(c)) {
                                stringAddChar(&token->data, c);

                                state = sDoubleExponentNumber;
                        } else {
                                state = sLexError;
                        }
                        break;
                /************** sDoubleExponentOperator End ****************/

                /************** sDoubleExponentNumber Start ****************/
                case sDoubleExponentNumber:
                        if(isdigit(c)) {
                                stringAddChar(&token->data, c);

                                state = sDoubleExponentNumber;
                        } else {
                                /* koniec number - retrurn double exponent */
                                ungetc(c, stdin);

                                token->type = sNumber;
                                token->subtype = sDoubleExponentNumber;
                                return token;
                        }
                        break;
                /************** sDoubleExponentNumber End ****************/

                /************** sDoublePointNumber Start ****************/
                case sDoublePointNumber:
                        if(isdigit(c)) {
                                stringAddChar(&token->data, c);

                                state = sDoublePointNumber;
                        } else
                        if((c == 'E') || (c == 'e')) {
                                stringAddChar(&token->data, c);

                                state = sDoubleExponent;
                        } else {
                                /* koniec number - return double point */
                                ungetc(c, stdin);

                                token->type = sNumber;
                                token->subtype = sDoublePointNumber;
                                return token;
                        }
                        break;
                /************** sDoubleExponentNumber End ****************/
                /********* Number End ******************/


                /********* Operators Start *******************************/
                /************** sMore Start ****************/
                case sMore:
                  if(c == '=') {
                    stringAddChar(&token->data, c);
                    state = sMoreEqual;
                  } else {
                    ungetc(c, stdin);

                    token->type = sOperand;
                    token->subtype = sMore;
                    return token;
                  }
                        break;
                /************** sMore End ****************/

                /************** sMoreEqual Start ****************/
                case sMoreEqual:
                    ungetc(c, stdin);

                    token->type = sOperand;
                    token->subtype = sMoreEqual;
                    return token;
                        break;
                /************** sMoreEqual End ****************/

                /************** sLess Start ****************/
                case sLess:
                  if(c == '=') {
                    stringAddChar(&token->data, c);
                    state = sLessEqual;
                  } else {
                    ungetc(c, stdin);

                    token->type = sOperand;
                    token->subtype = sLess;
                    return token;
                  }
                        break;
                /************** sLess End ****************/

                /************** sLessEqual Start ****************/
                case sLessEqual:
                    ungetc(c, stdin);

                    token->type = sOperand;
                    token->subtype = sLessEqual;
                    return token;
                        break;
                /************** sLessEqual End ****************/

                /************** sInequal Start ****************/
                case sInequal:
                  if(c == '=') {
                    stringAddChar(&token->data, c);

                    token->type = sOperand;
                    token->subtype = sInequal;
                    return token;
                  } else {
                    state = sLexError;
                  }
                        break;
                /************** sInequal End ****************/

                /************** sAssignment Start ****************/
                case sAssignment:
                  if(c == '=') {
                    stringAddChar(&token->data, c);
                    state = sEqual;
                  } else {
                    ungetc(c, stdin);

                    token->type = sOperand;
                    token->subtype = sAssignment;
                    return token;
                  }
                        break;
                /************** sAssignment End ****************/

                /************** sEqual Start ****************/
                case sEqual:
                    ungetc(c, stdin);

                    token->type = sOperand;
                    token->subtype = sEqual;
                    return token;
                        break;
                /************** sEqual End ****************/

                /************** sDivideFloat Start ****************/
                case sDivideFloat:
                  if(c == '/') {
                    stringAddChar(&token->data, c);
                    state = sDivideInteger;
                  } else {
                    ungetc(c, stdin);

                    token->type = sOperand;
                    token->subtype = sDivideFloat;
                    return token;
                  }
                        break;
                /************** sDivideFloat End ****************/

                /************** sDivideInteger Start ****************/
                case sDivideInteger:
                    ungetc(c, stdin);

                    token->type = sOperand;
                    token->subtype = sDivideInteger;
                    return token;
                        break;
                /************** sDivideInteger End ****************/

                /************** sMultiplication Start ****************/
                case sMultiplication:
                    ungetc(c, stdin);

                    token->type = sOperand;
                    token->subtype = sMultiplication;
                    return token;
                        break;
                /************** sMultiplication End ****************/

                /************** sMinus Start ****************/
                case sMinus:
                    ungetc(c, stdin);

                    token->type = sOperand;
                    token->subtype = sMinus;
                    return token;
                        break;
                /************** sMinus End ****************/

                /************** sPlus Start ****************/
                case sPlus:
                    ungetc(c, stdin);

                    token->type = sOperand;
                    token->subtype = sPlus;
                    return token;
                        break;
                /************** sPlus End ****************/

                /************** sComma Start ****************/
                case sComma:
                    ungetc(c, stdin);

                    token->type = sComma;
                    return token;
                        break;
                /************** sComma End ****************/

                /************** sLeftBracket Start ****************/
                case sLeftBracket:
                    ungetc(c, stdin);

                    token->type = sOperand;
                    token->subtype = sLeftBracket;
                    return token;
                        break;
                /************** sLeftBracket End ****************/

                /************** sRightBracket Start ****************/
                case sRightBracket:
                    ungetc(c, stdin);

                    token->type = sOperand;
                    token->subtype = sRightBracket;
                    return token;
                        break;
                /************** sRightBracket End ****************/
                /********* Operators End *******************************/


                /* compatibility issues */
                case sDef:
                case sElse:
                case sIf:
                case sNone:
                case sPass:
                case sReturn:
                case sWhile:
                case sString:
                case sNumber:
                case sInteger:
                case sOperand: break;

                case sLexError:
                  stringAddString(&token->data, "Lexikálne chyba\n");
                  token->type = sLexError;
                  token->line = line_cnt;
                 break;

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

/* funkcia uloží value na koniec zásobníka */
void stackPush(tStack *s, int value) {
        /* nájdi vhodné miesto na vloženie */
        int level = 1;
        while(s[level] != 0) {
                level++;
        }
        s[level] = value;
}

/* funkcia vymaže poslenú položku */
void stackPop(tStack *s) {
        /* nájdi poslednú položku */
        int level = 1;
        while(s[level] != 0) {
                level++;
        }
        s[level] = 0;
}

/* funkcia vráti hodnotu poslednej položky */
int stackTop(tStack *s) {
        /* nájdi poslednú položku */
        int level = 1;
        while(s[level] != 0) {
                level++;
        }
        return s[level];
}

bool stackEmpty(tStack* s) {
        return (s[1] == 0) ? (true) : (false);
}
