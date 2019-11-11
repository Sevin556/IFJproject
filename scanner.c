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
        int indent_cnt = 0;

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
                                token.type = sEOF;

                                return token;
                        } else

                        /* End Of Line */
                        if(c == '\n') {
                                if(FirstToken) {
                                        state = sStart;
                                } else {
                                        FirstToken = true;

                                        stringAddChar(&token.data, '\n');
                                        token.type = sEOL;
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
                        }

                        break;
                /**************koniec počiatočného stavu*************/
                
                case sEOL:
                }
        }
}
