
#ifndef Token_h
#define Token_h
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct token {
    char tName[256];
    char lName[256];
    int index;
    int line;
    int column;
} token;

//globals
token* TokenList[256];
int ctInd = 0, tInd = 0;

//protos
void setupTokenList();
void printTokenList();
token* getNextToken();


token* newToken(char lex[], char tn[], int i, int l, int c);

//definitions
token* newToken(char lex[], char tn[], int i, int l, int c) {
    token* temp = (token*)malloc(sizeof(token));
    strcpy(temp->tName, tn);
    strcpy(temp->lName, lex);
    temp->index = i;
    temp->line = l;
    temp->column = c;
    return temp;
}

void setupTokenList() {
    //setup token list from tokens.txt
    char lex[256], tok[256];
    int index, line, col;
    FILE* f = fopen("tokens.txt", "r");
    fscanf(f, "%d\n", &tInd);
    for ( int i = 0; i <= tInd; i++ ) {
        fscanf(f, "%s %s %d %d %d\n", lex, tok, &index, &line, &col);
        TokenList[i] = newToken(lex, tok, index, line, col);
    }
    //add EOF token Manually.
    TokenList[tInd] = newToken("EOF", "EOF", 0, 0, 0);
}

void printTokenList() {
    printf("\n");
    printf("      LEX  |   TOKEN   | INDEX | LN | COL  \n");
    printf("-----------------------------------------------\n");
    for ( int i = 0; i < tInd; i++ ) {
        printf("<%10s %10s %6d %3d %3d >\n", TokenList[i]->lName, TokenList[i]->tName, TokenList[i]->index, TokenList[i]->line, TokenList[i]->column);
    }
}

token* getNextToken() {
    if ( ctInd <= tInd ) {
        return TokenList[ctInd++]; 
    } else {
        return TokenList[tInd];
    }
}


#endif
