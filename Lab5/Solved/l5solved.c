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

token* TokenList[256];
token* lookahead;
int ctInd = 0, tInd = 0;

void proc_t();
void proc_e();
void setupTokenList();
void printTokenList();

int main () {
    setupTokenList();
    printTokenList();
    proc_e();
    return 0;
}

void proc_e() {
    lookahead = TokenList[ctInd++];

    if (strcmp(lookahead->tName, "NUMBER") == 0 ) {
        proc_t();
    } else {
        printf("Error\n");
    }

    if (tInd == ctInd) {
        printf("Success (Match)\n");
    } else {
        printf("Error\n");
    } 
}

void proc_t() {
    lookahead = TokenList[ctInd++];
    if ( strcmp(lookahead->tName, "MUL") == 0) {
        lookahead = TokenList[ctInd++];
        if( strcmp(lookahead->tName, "NUMBER") == 0 ) {
            lookahead = TokenList[ctInd++];
            proc_t();
        } else {
            printf("Error\n");
        }
    }
}

void setupTokenList() {
    //setup token list from tokens.txt
    char lex[256], tok[256];
    int index, line, col;
    FILE* f = fopen("tokens.txt", "r");
    fscanf(f, "%d\n", &tInd);
    for ( int i = 0; i < tInd; i++ ) {
        fscanf(f, "%s %s %d %d %d\n", lex, tok, &index, &line, &col);
        TokenList[i] = newToken(lex, tok, index, line, col);
    }
}

void printTokenList() {
    printf("\n");
    printf("      LEX  |   TOKEN   | INDEX | LN | COL  \n");
    printf("-----------------------------------------------\n");
    for ( int i = 0; i < tInd; i++ ) {
        printf("<%10s %10s %6d %3d %3d >\n", TokenList[i]->lName, TokenList[i]->tName, TokenList[i]->index, TokenList[i]->line, TokenList[i]->column);
    }
}
