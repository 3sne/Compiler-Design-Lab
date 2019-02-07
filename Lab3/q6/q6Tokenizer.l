%{
#include <stdlib.h>
#include <string.h>
typedef struct token {
    char tName[256];
    char lName[256];
} token;

char tokenName[][256] = {
    "RLOP",
    "IDENTIFIER",
    "AROP",
    "SS"
};

token* TokenList[1024];
int tInd = 0;

token* newToken(char lex[], char tn[]);
void printTokenList();
%}

%%

(("#".*)|("//".*))"\n"                              { }
"<"|">"|"<="|">="|"=="|"!="                         { TokenList[tInd++] = newToken(yytext, tokenName[0]); }
"+"|"-"|"*"|"/"                                     { TokenList[tInd++] = newToken(yytext, tokenName[2]); }


%%

int main() {
    yyin = fopen("input.c", "r");
    yylex();
    printTokenList();
}

token* newToken(char lex[], char tn[]) {
    token* temp = (token*)malloc(sizeof(token));
    strcpy(temp->tName, tn);
    strcpy(temp->lName, lex);
    return temp;
}

void printTokenList() {
    printf("\n");
    for ( int i = 0; i < tInd; i++ ) {
        printf("<%10s , %10s>\n", TokenList[i]->lName, TokenList[i]->tName);
    }
}