/*
    G:
        E -> numT
        T -> *numT|<epsilon>
*/
#include "token.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int matchFlag = 1;
token* lookahead;

void proc_t();
void proc_e();

int main () {
    setupTokenList();
    printTokenList();
    proc_e();
    if ( matchFlag ) {
        printf("Verdict: ACCEPTED.");
    } else {
        printf("Verdict: DENIED.");
    }

    return 0;
}

void proc_e() {
    lookahead = getNextToken();
    if ( strcmp(lookahead->tName, "NUMBER") == 0 ) {
        proc_t();
    } else {
        matchFlag = 0;
        // printf("ERROR1");
    }

    if ( strcmp(lookahead->tName, "EOTL") == 0 && matchFlag == 1) {
        printf("SUCCESS");
    }
}

void proc_t() {
    lookahead = getNextToken();
    if ( strcmp(lookahead->tName, "MUL") == 0) {
        lookahead = getNextToken();
        if( strcmp(lookahead->tName, "NUMBER") == 0 ) {
            proc_t();
        } else {
            matchFlag = 0;
            // printf("ERROR2\n");
        }
    }
}
