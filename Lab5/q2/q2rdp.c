/*
    @Author:        3sne ( Mukur Panchani )
    @FileName:      q2rdp.c
    @Task:          RDP for given grammar G
    @Dependency:    token.h tokenizer.exe input.txt

    G:
        E -> E+T|T
        T -> T*F|F
        F -> (E)|id

    This grammar has left-recursion. RDP don't work well with left-recursive grammar since long strings may lead to long call stack.
    Converting it to equivalent G',

    G':
        E -> TE'
        E'-> +TE'|<epsilon>
        T -> FT'
        T'-> *FT'|<epsilon>
        F -> (E)|id
*/
#include "token.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int matchFlag = 1;
token* lookahead;

void E();
void Edash();
void T();
void Tdash();
void F();

int main () {
    setupTokenList();
    printTokenList();
    E();
    lookahead = getNextToken(); //to confirm all tokens are acutally exhausted via EOTL. Otherwise, partial matches.
    if ( matchFlag && strcmp(lookahead->tName, "EOTL") == 0 ) {
        printf("Verdict: ACCEPTED.");
    } else {
        printf("Verdict: DENIED.");
    }
    return 0;
}

void E() {
    T();
    Edash();
    return;
}

void Edash() {
    lookahead = getNextToken();
    if ( strcmp(lookahead->lName, "+") == 0 ) {
        T();
        Edash();
        return;
    }
    retract();
}

void T() {
    F();
    Tdash();
}

void Tdash() {
    lookahead = getNextToken();
    if ( strcmp(lookahead->lName, "*") == 0 ) {
        F();
        Tdash();
        return;
    }
    retract();
}

void F() {
    lookahead = getNextToken();
    if ( strcmp(lookahead->lName, "(") == 0 ) {
        E();
        lookahead = getNextToken();
        if ( strcmp(lookahead->lName, ")") == 0 ) {
            return;
        } else {
            matchFlag = 0;
        } 
    } else if ( strcmp(lookahead->tName, "IDENTIFIER") == 0 ) {
        return;
    } else {
        matchFlag = 0;
    }
}