
#ifndef Token_h
#define Token_h
#include <stdlib.h>
#include <string.h>

typedef struct token {
    char tName[256];
    char lName[256];
    int index;
    int line;
    int column;
} token;

token* newToken(char lex[], char tn[], int i, int l, int c) {
    token* temp = (token*)malloc(sizeof(token));
    strcpy(temp->tName, tn);
    strcpy(temp->lName, lex);
    temp->index = i;
    temp->line = l;
    temp->column = c;
    return temp;
}

#endif