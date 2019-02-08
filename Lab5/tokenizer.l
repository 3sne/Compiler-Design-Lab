%{
#include <stdlib.h>
#include <string.h>
typedef struct token {
    char tName[256];
    char lName[256];
    int index;
    int line;
    int column;
} token;

char tokenName[][256] = {
    "RLOP",
    "IDENTIFIER",
    "AROP",
    "SS",
    "LP",
    "RP",
    "LC",
    "RC",
    "KEYWORD",
    "NUMBER",
    "ASSIGN",
    "LITERAL",
    "LOGOP",
    "LSB",
    "RSB"
};

token* TokenList[1024];
int tInd = 0;
int gl = 1, gc = 1;

token* newToken(char lex[], char tn[], int i, int l, int c);
void printTokenList();
void putTokenListInFile();
%}

%%

"\t"                                                { gc += 4; }
" "                                                 { gc += 1; }
"\n"                                                { gl += 1; gc = 1; }
(("#".*)|("//".*))"\n"                              { gc += strlen(yytext); gl += 1;}
("/*"(.|"\n")*"*/")                                 { int cnt = 0;
                                                      while (yytext[cnt] != '\0') {
                                                          if ( yytext[cnt] == '\n' ) {
                                                              gl += 1;
                                                          }
                                                          cnt++;
                                                      }
                                                    }
"\"".*"\""                                          { TokenList[tInd++] = newToken(yytext, tokenName[11], 0, gl, gc); gc += strlen(yytext);}
";"|","                                             { TokenList[tInd++] = newToken(yytext, tokenName[3], 0, gl, gc); gc += strlen(yytext);}
"="                                                 { TokenList[tInd++] = newToken(yytext, tokenName[10], 0, gl, gc); gc += strlen(yytext);}
"<"|">"|"<="|">="|"=="|"!="                         { TokenList[tInd++] = newToken(yytext, tokenName[0], 0, gl, gc); gc += strlen(yytext);}
"+"                                                 { TokenList[tInd++] = newToken(yytext, "ADD", 0, gl, gc); gc += strlen(yytext);}
"*"                                                 { TokenList[tInd++] = newToken(yytext, "MUL", 0, gl, gc); gc += strlen(yytext);}
"-"|"/"                                             { TokenList[tInd++] = newToken(yytext, tokenName[2], 0, gl, gc); gc += strlen(yytext);}
"&"|"&&"|"|"|"||"                                   { TokenList[tInd++] = newToken(yytext, tokenName[12], 0, gl, gc); gc += strlen(yytext);}
"("                                                 { TokenList[tInd++] = newToken(yytext, tokenName[4], 0, gl, gc); gc += strlen(yytext);}
")"                                                 { TokenList[tInd++] = newToken(yytext, tokenName[5], 0, gl, gc); gc += strlen(yytext);}
"{"                                                 { TokenList[tInd++] = newToken(yytext, tokenName[6], 0, gl, gc); gc += strlen(yytext);}
"}"                                                 { TokenList[tInd++] = newToken(yytext, tokenName[7], 0, gl, gc); gc += strlen(yytext);}
"["                                                 { TokenList[tInd++] = newToken(yytext, tokenName[13], 0, gl, gc); gc += strlen(yytext);}
"]"                                                 { TokenList[tInd++] = newToken(yytext, tokenName[14], 0, gl, gc); gc += strlen(yytext);}
"if"|"break"|"else"|"long"|"switch"|"case"|"enum"|"register"|"typedef"|"char"|"extern"|"return"|"union"|"const"|"float"|"short"|"unsigned"|"continue"|"for"|"signed"|"void"|"default"|"goto"|"sizeof"|"do"|"static"|"while"|"int"                                      { TokenList[tInd++] = newToken(yytext, tokenName[8], 0, gl, gc); gc += strlen(yytext);}
([a-zA-Z_])([a-zA-Z_0-9])*                          { TokenList[tInd++] = newToken(yytext, tokenName[1], 0, gl, gc); gc += strlen(yytext);}
[0-9]+("."?[0-9]+)?                                 { TokenList[tInd++] = newToken(yytext, tokenName[9], 0, gl, gc); gc += strlen(yytext);}


%%

int main() {
    yyin = fopen("input.txt", "r");
    yylex();
    printTokenList();
    putTokenListInFile();
    fclose(yyin);
}

token* newToken(char lex[], char tn[], int i, int l, int c) {
    token* temp = (token*)malloc(sizeof(token));
    strcpy(temp->tName, tn);
    strcpy(temp->lName, lex);
    temp->index = i;
    temp->line = l;
    temp->column = c;
    return temp;
}

void printTokenList() {
    printf("\n");
    for ( int i = 0; i < tInd; i++ ) {
        printf("<%10s %10s   %3d %3d %3d >\n", TokenList[i]->lName, TokenList[i]->tName, TokenList[i]->index, TokenList[i]->line, TokenList[i]->column);
    }
}

void putTokenListInFile() {
    FILE *o = fopen("tokens.txt", "w");
    fprintf(o, "%d\n", tInd);
    for ( int i = 0; i < tInd; i++ ) {
        fprintf(o, "%s %s %d %d %d\n", TokenList[i]->lName, TokenList[i]->tName, TokenList[i]->index, TokenList[i]->line, TokenList[i]->column);
    }
    fclose(o);
}
