/*
    @Author:        3sne ( Mukur Panchani )
    @FileName:      q1rdp.c
    @Task:          RDP for given grammar G
    @Dependency:    token.h tokenizer.exe input.txt
    G:
        S -> a|>|(T)
        T -> T,S|S

    This grammar has left-recursion. RDP don't work well with left-recursive grammar since long strings may lead to long call stack.
    Converting it to equivalent G',

    G':
        S -> a|>|(T)
        T -> ST'
        T'-> ,ST'|<epsilon>
*/
#include "token.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int matchFlag = 1;
token* lookahead;

void proc_s();
void proc_t();
void proc_tdash();

int main () {
    setupTokenList();
    printTokenList();
    proc_s();
    lookahead = getNextToken(); //to confirm all tokens are acutally exhausted via EOTL. Otherwise, partial matches.
    if ( matchFlag && strcmp(lookahead->tName, "EOTL") == 0 ) {
        printf("Verdict: ACCEPTED.");
    } else {
        printf("Verdict: DENIED.");
    }
    return 0;
}

void proc_s() {
    lookahead = getNextToken();
    
    if ( strcmp(lookahead->tName, "EOTL") == 0 && matchFlag == 1) {
        printf("SUCCESS");
        return;
    }
    
    if ( strcmp(lookahead->lName, "a") == 0 ) {
        return;
    } else if (strcmp(lookahead->lName, ">") == 0 ) {
        return;
    } else if (strcmp(lookahead->lName, "(") == 0 ) {
        proc_t();
        lookahead = getNextToken();
        if ( strcmp(lookahead->lName, ")") == 0 ) {
            return;
        } else {
            matchFlag = 0;
            printf("e2\n");
        }
    } else {
        matchFlag = 0;
        printf("e1\n");
    }

}

void proc_t() {
    proc_s();
    proc_tdash();
}

void proc_tdash() {
    lookahead = getNextToken();
    if ( strcmp(lookahead->lName, ",") == 0) {
        proc_s();
        proc_tdash();
        return;         //this return is important, think.
    }
    retract();
}