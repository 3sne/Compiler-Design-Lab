/*
    @Author:        3sne ( Mukur Panchani )
    @FileName:      q3rdp.c
    @Task:          RDP for given grammar G
    @Dependency:    token.h tokenizer.exe input.txt

    G:
        S -> aAcBe
        A -> Ab|b
        B -> d

    This grammar has left-recursion. RDP don't work well with left-recursive grammar since long strings may lead to long call stack.
    Converting it to equivalent G',

    G':
        S -> aAcBe
        A -> bA'
        A'-> bA'|<epsilon>
        B -> d
*/
#include "token.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int matchFlag = 1;
token* lookahead;

void S();
void A();
void Adash();
void B();

int main () {
    setupTokenList();
    printTokenList();
    S();
    lookahead = getNextToken(); //to confirm all tokens are acutally exhausted via EOTL. Otherwise, partial matches.
    if ( matchFlag && strcmp(lookahead->tName, "EOTL") == 0 ) {
        printf("Verdict: ACCEPTED.");
    } else {
        printf("Verdict: DENIED.");
    }
    return 0;
}

void S() {
    lookahead = getNextToken();
    if ( strcmp(lookahead->lName, "a") == 0 ) {
        A();
        lookahead = getNextToken();
        if ( strcmp(lookahead->lName, "c") == 0 ) {
            B();
            lookahead = getNextToken();
            if ( strcmp(lookahead->lName, "e") == 0 ) {
                return;
            } else {
                matchFlag = 0;
                printf("e3\n");
            }
        } else {
            matchFlag = 0;
            printf("e2\n");
        }
    } else {
        matchFlag = 0;
        printf("e1\n");
    }
}

void A() {
    lookahead = getNextToken();   
    if ( strcmp(lookahead->lName, "b") == 0 ) {
        Adash();
        return;
    } else {
        matchFlag = 0;
        printf("e4\n");
    }
}

void Adash() {
    lookahead = getNextToken();
    if ( strcmp(lookahead->lName, "b") == 0 ) {
        Adash();
        return;
    }
    retract();
}

void B() {
    lookahead = getNextToken();
    if ( strcmp(lookahead->lName, "d") == 0 ) {
        return;
    } else {
        matchFlag = 0;
        printf("e5\n");
    }
}