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
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>

tToken get_token(void) {
        tState state;
        tToken token;
        bool FirstToken;
        char c;
        static int indent_cnt = 0;
        static int line_cnt = 0;

        state = sStart;       /* počiatocný stav */
        stringInit(&token.data); /* inicializácia stringu */

        while(true) {
                c = getchar();

                switch(state) {
                case sStart:
                        /****************počiatocný stav********************/
                        /* End Of File */
                        if(c == EOF) {
                                stringAddString(&token.data, "EOF");
                                token.line = line_cnt;
                                token.type = sEOF;

                                return token;
                        }

                        /* End Of Line */
                        if(c == '\n') {
                                if(FirstToken) {
                                        line_cnt++;
                                        state = sStart;
                                } else {
                                        FirstToken = true;
                                        line_cnt++;

                                        stringAddChar(&token.data, '\n');
                                        token.line = line_cnt;
                                        token.type = sEOL;
                                }
                        }

                        /* space */
                        if(isspace(c)) {
                                if(FirstToken) {
                                        state = sDentDecide;
                                        indent_cnt++;         /* zvýšenie počítadla indentu */
                                        FirstToken = false;
                                } else {
                                        state = sStart;
                                }
                        }

                        /* line comment */
                        if(c == '#') {
                                state = sLineComment;
                        }

                        /* block comment */
                        if(c == '"') {
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
                        } else {
                                state = sLexError;
                        }

                        /* id/keyword */
                        if(c == '\'') {
                                token.line = line_cnt;

                                state = sStringStart;
                        }

                        break;
                /**************koniec počiatočného stavu*************/



                /************** EOL Start ****************/
                case sEOL:

                        break;
                /************** EOL Start ****************/

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

                /************** String Start Start (lol)****************/
                case sStringStart:

                        if(c == '\'') {
                                /* prázdny string */
                                stringAddString(&token.data, "");
                                token.type = sString;
                        } else
                        if(c == '\\') {
                                /* escape sequence */
                                state = sStringEscape;
                        } else
                        if(c > 31) {
                                /* literál */
                                stringAddChar(&token.data, c);
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
                                stringAddChar(&token.data, '\n');
                                state = sStringRead;
                                break;

                        case 't':
                                stringAddChar(&token.data, '\t');
                                state = sStringRead;
                                break;

                        case '\'':
                                stringAddChar(&token.data, '\'');
                                state = sStringRead;
                                break;

                        case '\"':
                                stringAddChar(&token.data, '\"');
                                state = sStringRead;
                                break;

                        case '\\':
                                stringAddChar(&token.data, '\\');
                                state = sStringRead;
                                break;

                        case 'x':
                                state = sStringEscapeHex;
                                break;

                        default:
                                stringAddChar(&token.data, '\\');
                                stringAddChar(&token.data, c);
                                state = sStringRead;
                        }

                        break;
                        /************** String Escape Start ****************/
                }
        }
}
