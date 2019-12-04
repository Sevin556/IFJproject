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
#include "string.h"

/* definícia premenných */
int line_cnt = 1;
bool FirstToken = true;
bool FirstInit = true;
tIndentStack stackI[MAX_DENT];

tToken* get_token(void) {
        tState state;
        char c;
        if(FirstInit) {
                stackInitI(stackI);
                FirstInit = false;
        }
        
        tToken *token = init_token();

        int indent_cnt = 0;

        state = sStart;       /* počiatocný stav */

        while(true) {
                c = getchar();

                switch(state) {
                case sStart:
                        /****************počiatocný stav********************/
                        /* check Dedent */
                        if(FirstToken && c != ' ') {
                                if(stackEmptyI(stackI)) {
                                        /* no indentation */
                                        FirstToken = false;
                                } else {
                                        /* dedent all needed */
                                        ungetc(c, stdin);
                                        stackPopI(stackI);
                                        state = sDedent;
                                        
                                        break;
                                }
                        }

                        /* End Of File */
                        if(c == EOF) {
                                state = sEOF;
                                break;
                        } else

                        /* End Of Line */
                        if(c == '\n') {
                                if(FirstToken) {
                                        line_cnt++;
                                        state = sStart;
                                        break;
                                } else {
                                        FirstToken = true;
                                        line_cnt++;

                                        /*stringAddChar(&(token->data), '\n');*/
                                        stringAddString(&(token->data), "\\n"); //DEBUG
                                        token->line = line_cnt;
                                        token->type = sEOL;
                                        return token;
                                }
                        } else

                        /* space */
                        if(isspace(c)) {
                                if(FirstToken) {
                                        indent_cnt++;         /* zvýšenie počítadla indentu */
                                        state = sDentDecide;
                                        break;
                                } else {
                                        state = sStart;
                                        break;
                                }
                        } else

                        /* line comment */
                        if(c == '#') {
                                state = sLineComment;
                                break;
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
                                                break;
                                        } else {
                                                ungetc(c, stdin);
                                                ungetc(c, stdin);
                                                state = sLexError;
                                                break;
                                        }
                                } else {
                                        ungetc(c, stdin);
                                        state = sLexError;
                                        break;
                                }
                        } else

                        /* string */
                        if(c == '\'') {
                                token->line = line_cnt;

                                state = sStringStart;
                                break;
                        } else

                        /* id/key */
                        if(isalpha(c)) {
                                token->line = line_cnt;
                                stringAddChar(&(token->data), c);

                                state = sIdentificatorOrKeyword;
                                break;
                        } else
                        if(c == '_') {
                                token->line = line_cnt;
                                stringAddChar(&(token->data), c);

                                state = sIdentificator;
                                break;
                        } else

                        /* number */
                        if(isdigit(c)) {
                                token->line = line_cnt;
                                stringAddChar(&(token->data), c);

                                state = sIntegerOrDouble;
                                break;
                        } else

                        /* colon */
                        if(c == ':') {
                                token->line = line_cnt;
                                stringAddChar(&(token->data), c);

                                state = sColon;
                                break;
                        } else

                        /* operators */
                        if(c == '+') {
                                token->line = line_cnt;
                                stringAddChar(&(token->data), c);

                                state = sPlus;
                                break;
                        } else
                        if(c == '-') {
                                token->line = line_cnt;
                                stringAddChar(&(token->data), c);

                                state = sMinus;
                                break;
                        } else
                        if(c == '*') {
                                token->line = line_cnt;
                                stringAddChar(&(token->data), c);

                                state = sMultiplication;
                                break;
                        } else
                        if(c == '/') {
                                token->line = line_cnt;
                                stringAddChar(&(token->data), c);

                                state = sDivideFloat;
                                break;
                        } else
                        if(c == '>') {
                                token->line = line_cnt;
                                stringAddChar(&(token->data), c);

                                state = sMore;
                                break;
                        } else
                        if(c == '<') {
                                token->line = line_cnt;
                                stringAddChar(&(token->data), c);

                                state = sLess;
                                break;
                        } else
                        if(c == '!') {
                                token->line = line_cnt;
                                stringAddChar(&(token->data), c);

                                state = sInequal;
                                break;
                        } else
                        if(c == '=') {
                                token->line = line_cnt;
                                stringAddChar(&(token->data), c);

                                state = sAssignment;
                                break;
                        } else
                        if(c == '(') {
                                token->line = line_cnt;
                                stringAddChar(&(token->data), c);

                                state = sLeftBracket;//TODO
                                break;
                        } else
                        if(c == ')') {
                                token->line = line_cnt;
                                stringAddChar(&(token->data), c);

                                state = sRightBracket;
                                break;
                        } else
                        if(c == ',') {
                                token->line = line_cnt;
                                stringAddChar(&(token->data), c);

                                state = sComma;
                                break;
                        }

                        break;
                /**************koniec počiatočného stavu*************/

                /************** sEOF Start ****************/
                case sEOF:
                        stringAddString(&(token->data), "EOF");
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
                                break;
                        } else
                        if(c == '\n') {
                                state = sStart;
                                break;
                        } else {
                                ungetc(c, stdin);
                                FirstToken = false;

                                int curr = stackTopI(stackI);
                                if(indent_cnt > curr) {
                                        stackPushI(stackI, indent_cnt);
                                        state = sIndent;
                                        break;
                                } else 
                                if(indent_cnt < curr) {
                                        stackPopI(stackI);
                                        curr = stackTopI(stackI);
                                        if(indent_cnt == curr) {
                                                /* správny dedent */
                                                state = sDedent;
                                        } else {
                                                /* nesprávny dedent */
                                                state = sLexError;
                                        }
                                } else {
                                        /* indent_cnt == curr => netreba indent */
                                        state = sStart;
                                }
                        }
                        break;
                /************** sDentDecide End ****************/

                /************** sIndent Start ****************/
                case sIndent:
                        ungetc(c, stdin);
                        stringAddString(&(token->data), "INDENT"); //DEBUG
                        token->type = sIndent;
                        token->line = line_cnt;
                        return token;
                        break;
                /************** sIndent End ****************/

                /************** sDedent Start ****************/
                case sDedent:
                        ungetc(c, stdin);
                        stringAddString(&(token->data), "DEDENT");      //DEBUG
                        token->type = sDedent;
                        token->line = line_cnt; 
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
                        token->type = sString;
                        if(c == '\'') {
                                /* prázdny string */
                                stringAddString(&(token->data), "");
                                return token;
                        } else
                        if(c == '\\') {
                                /* escape sequence */
                                state = sStringEscape;
                                break;
                        }
                        else if ((c <= 32) || (c == 35) || (c == 92))
                        {
                                /* biele znaky a # a / */
                                char temp[6];
                                sprintf(temp, "\\0%d", c);
                                stringAddString(&(token->data), temp);
                                state = sStringRead;
                                break;
                        }
                        else if (c > 32)
                        {
                                /* literál */
                                stringAddChar(&(token->data), c);
                                state = sStringRead;
                                break;
                        } else {
                                /* error */
                                state = sLexError;
                                break;
                        }

                        break;
                /************** sStringStart End (lol) ****************/

                /************** sStringEscape Start ****************/
                case sStringEscape:

                        switch(c) {
                        case 'n':
                                stringAddChar(&(token->data), '\n');
                                state = sStringRead;
                                break;

                        case 't':
                                stringAddChar(&(token->data), '\t');
                                state = sStringRead;
                                break;

                        case '\'':
                                stringAddChar(&(token->data), '\'');
                                state = sStringRead;
                                break;

                        case '\"':
                                stringAddChar(&(token->data), '\"');
                                state = sStringRead;
                                break;

                        case '\\':
                                stringAddString(&(token->data), "\092");
                                state = sStringRead;
                                break;

                        case 'x':
                                state = sStringEscapeHex;
                                break;

                        default:
                                stringAddString(&(token->data), "\092");
                                if ((c <= 32) || (c == 35) || (c == 92))
                                {
                                        /* biele znaky a # a / */
                                        char temp[6];
                                        sprintf(temp, "\\0%d", c);
                                        stringAddString(&(token->data), temp);
                                        state = sStringRead;
                                        break;
                                } else {
                                        stringAddChar(&(token->data), c);
                                        state = sStringRead;
                                }
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
                                        stringAddChar(&(token->data), temp);
                                        stringAddChar(&(token->data), c);
                                        state = sStringRead;
                                } else {
                                        /* escape failed - return \xx */
                                        stringAddChar(&(token->data), '\\');
                                        stringAddChar(&(token->data), temp);
                                        stringAddChar(&(token->data), c);
                                        state = sStringRead;
                                }

                        } else {
                                /* escape failde return \x */
                                stringAddChar(&(token->data), '\\');
                                stringAddChar(&(token->data), 'x');
                                stringAddChar(&(token->data), c);
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
                                break;
                        } else
                        if((c <= 32) || (c == 35) || (c == 92)) {
                                /* biele znaky a # a / */
                                char temp[6];
                                sprintf(temp, "\\0%d", c);
                                stringAddString(&(token->data), temp);
                                state = sStringRead;
                                break;
                        } else
                        if(c > 32) {
                                /* literál */
                                stringAddChar(&(token->data), c);
                                state = sStringRead;
                                break;
                        } else {
                                /* error */
                                state = sLexError;
                                break;
                        }

                        break;
                /************** sStringRead End ****************/
                /*********String End *****************************/


                /********* Keyword / ID Start ******************/
                /************** sIdentificatorOrKeyword Start ****************/
                case sIdentificatorOrKeyword:
                        if(isalpha(c)) {
                                stringAddChar(&(token->data), c);
                                state = sIdentificatorOrKeyword;
                        } else
                        if(isdigit(c) || c == '_') {
                                stringAddChar(&(token->data), c);
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
                                stringAddChar(&(token->data), c);
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
                                stringAddChar(&(token->data), c);

                                state = sIntegerOrDouble;
                        } else
                        if(c == '.') {
                                stringAddChar(&(token->data), c);

                                state = sDoublePoint;
                        } else
                        if((c == 'E') || (c == 'e')) {
                                stringAddChar(&(token->data), c);

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
                                stringAddChar(&(token->data), c);

                                state = sDoublePointNumber;
                        } else {
                                state = sLexError;
                        }
                        break;
                /************** sDoublePoint End ****************/

                /************** sDoubleExponent Start ****************/
                case sDoubleExponent:
                        if(isdigit(c)) {
                                stringAddChar(&(token->data), c);

                                state = sDoubleExponentNumber;
                        } else
                        if((c == '+') || (c == '-')) {
                                stringAddChar(&(token->data), c);

                                state = sDoubleExponentOperator;
                        }
                        break;
                /************** sDoubleExponent End ****************/

                /************** sDoubleExponentOperator Start ****************/
                case sDoubleExponentOperator:
                        if(isdigit(c)) {
                                stringAddChar(&(token->data), c);

                                state = sDoubleExponentNumber;
                        } else {
                                state = sLexError;
                        }
                        break;
                /************** sDoubleExponentOperator End ****************/

                /************** sDoubleExponentNumber Start ****************/
                case sDoubleExponentNumber:
                        if(isdigit(c)) {
                                stringAddChar(&(token->data), c);

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
                                stringAddChar(&(token->data), c);

                                state = sDoublePointNumber;
                        } else
                        if((c == 'E') || (c == 'e')) {
                                stringAddChar(&(token->data), c);

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


                /************** sColon Start ****************/
                case sColon:
                        ungetc(c, stdin);

                        token->type = sColon;
                        return token;
                        break;
                /************** sColon End ****************/


                /********* Operators Start *******************************/
                /************** sMore Start ****************/
                case sMore:
                        if(c == '=') {
                                stringAddChar(&(token->data), c);
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
                                stringAddChar(&(token->data), c);
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
                                stringAddChar(&(token->data), c);

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
                                stringAddChar(&(token->data), c);
                                state = sEqual;
                        } else {
                                ungetc(c, stdin);

                                token->type = sAssignment;
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
                                stringAddChar(&(token->data), c);
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

                case sLexError:
                        //stringAddString(&(token->data), "Lexikálna chyba");
                        token->type = sLexError;
                        token->line = line_cnt;
                        return token;
                        break;

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
                case sInputS:
                case sInputI:
                case sInputF:
                case sPrint:
                case sLen:
                case sSubstr:
                case sOrd:
                case sChr:
                case sDollar:
                case sOperand: break;

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

/* vrátenie tokenu na vstup */
void unget_token(tToken* t) {
        for(int i = 0; i < t->data.length; i++) {
                ungetc(t->data.value[i], stdin);
        }
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
        } else
        if(strcmp(t->data.value, "inputs") == 0) {
                t->type = sInputS;
        } else
        if(strcmp(t->data.value, "inputi") == 0) {
                t->type = sInputI;
        } else
        if(strcmp(t->data.value, "inputf") == 0) {
                t->type = sInputF;
        } else
        if(strcmp(t->data.value, "print") == 0) {
                t->type = sPrint;
        } else
        if(strcmp(t->data.value, "len") == 0) {
                t->type = sLen;
        } else
        if(strcmp(t->data.value, "substr") == 0) {
                t->type = sSubstr;
        } else
        if(strcmp(t->data.value, "ord") == 0) {
                t->type = sOrd;
        } else
        if(strcmp(t->data.value, "chr") == 0) {
                t->type = sChr;
        } else {
                /* nieje keyword */
                t->type = sIdentificator;
        }
}

/* funkcia uloží value na koniec zásobníka */
void stackPushI(tIndentStack* s, int value) {
        /* nájdi vhodné miesto na vloženie */
        int level = 1;
        while(s[level] != -1) {
                level++;
        }
        s[level] = value;
}

/* funkcia vymaže poslenú položku */
void stackPopI(tIndentStack *s) {
        /* nájdi poslednú položku */
        int level = 1;
        while(s[level] != -1) {
                level++;
        }
        s[level - 1] = -1;
}

/* funkcia vráti hodnotu poslednej položky */
int stackTopI(tIndentStack *s) {
        /* nájdi poslednú položku */
        int level = 1;
        while(s[level] != -1) {
                level++;
        }
        return s[level - 1];
}

/* funkcia zistí či je stack prázdy */
bool stackEmptyI(tIndentStack* s) {
        return (s[1] == -1) ? (true) : (false);
}

/* inicializácia stacku */
void stackInitI(tIndentStack* s) {
        s[0] = 0;
        for(int i = 1; i < MAX_DENT; i++) {
                s[i] = -1;
        }
}

/* correct call - stringAddChar(&(token->data), 'o');*/