/* 
    Accepts strings belonging to following grammar G:
    G:
        E -> numT
        T -> *numT|<epsilon>
    
    This is how it works:
                                                   ------------------------------
    input.c (contain string to be checked by G) -> | lex.yy.c (from L3Q6 / L4Q1)| -> tokens.txt (input is tokenized) 
                                                   ------------------------------
                  -------------
    tokens.txt -> |thisProgram| -> Success/Failure
                  -------------
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
    return 0;
}

void proc_e() {
    lookahead = getNextToken();

    if (strcmp(lookahead->tName, "NUMBER") == 0 ) {
        proc_t();
    } else {
        matchFlag = 0;
        printf("Error\n");
    }

    if ( strcmp(lookahead->tName, "EOF") == 0 && matchFlag == 1) {
        printf("Success (Match)\n");
    } else {
        matchFlag = 0;
        printf("Failure (No Match)\n");
    } 
}

void proc_t() {
    lookahead = getNextToken();
    if ( strcmp(lookahead->tName, "MUL") == 0) {
        lookahead = getNextToken();
        if( strcmp(lookahead->tName, "NUMBER") == 0 ) {
            lookahead = getNextToken();
            proc_t();
        } else {
            matchFlag = 0;
            printf("Error\n");
        }
    }
}
