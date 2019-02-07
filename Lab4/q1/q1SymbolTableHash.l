%{
#include <stdlib.h>
#include <string.h>
#include "symtable.h"
#define LC '{'
#define RC '}'

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
symTableEntry* st[10];
int tInd = 0, scnt = -1;
int gl = 1, gc = 1;
int localizer = 0;


//functions skeletons
void init();
token* newToken(char lex[], char tn[], int i, int l, int c);
symTableEntry* newSte(char l[]);
int makeSTEForThisToken(token* currToken, int i);
void populateTokenIndices();
char getScope();
void printTokenList();
void printSymTable();
void printSymTableInHashTableFormat();
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
"\"".*"\""                                          { TokenList[tInd++] = newToken(yytext, tokenName[11], -1, gl, gc); gc += strlen(yytext);}
";"|","                                             { TokenList[tInd++] = newToken(yytext, tokenName[3], -1, gl, gc); gc += strlen(yytext);}
"="                                                 { TokenList[tInd++] = newToken(yytext, tokenName[10], -1, gl, gc); gc += strlen(yytext);}
"<"|">"|"<="|">="|"=="|"!="                         { TokenList[tInd++] = newToken(yytext, tokenName[0], -1, gl, gc); gc += strlen(yytext);}
"+"|"-"|"*"|"/"                                     { TokenList[tInd++] = newToken(yytext, tokenName[2], -1, gl, gc); gc += strlen(yytext);}
"&"|"&&"|"|"|"||"                                   { TokenList[tInd++] = newToken(yytext, tokenName[12], -1, gl, gc); gc += strlen(yytext);}
"("                                                 { TokenList[tInd++] = newToken(yytext, tokenName[4], -1, gl, gc); gc += strlen(yytext);}
")"                                                 { TokenList[tInd++] = newToken(yytext, tokenName[5], -1, gl, gc); gc += strlen(yytext);}
"{"                                                 { TokenList[tInd++] = newToken(yytext, tokenName[6], -1, gl, gc); gc += strlen(yytext);}
"}"                                                 { TokenList[tInd++] = newToken(yytext, tokenName[7], -1, gl, gc); gc += strlen(yytext);}
"["                                                 { TokenList[tInd++] = newToken(yytext, tokenName[13], -1, gl, gc); gc += strlen(yytext);}
"]"                                                 { TokenList[tInd++] = newToken(yytext, tokenName[14], -1, gl, gc); gc += strlen(yytext);}
"if"|"break"|"else"|"long"|"switch"|"case"|"enum"|"register"|"typedef"|"char"|"extern"|"return"|"union"|"const"|"float"|"short"|"unsigned"|"continue"|"for"|"signed"|"void"|"default"|"goto"|"sizeof"|"do"|"static"|"while"|"int"                                      { TokenList[tInd++] = newToken(yytext, tokenName[8], -1, gl, gc); gc += strlen(yytext);}
([a-zA-Z_])([a-zA-Z_0-9])*                          { printf("%s ", yytext); TokenList[tInd++] = newToken(yytext, tokenName[1], -2, gl, gc); gc += strlen(yytext);}
[0-9]+("."?[0-9]+)?                                 { TokenList[tInd++] = newToken(yytext, tokenName[9], -1, gl, gc); gc += strlen(yytext);}


%%

int main() {
    yyin = fopen("input.c", "r");
    yylex();
    
    //Now start assigning Token Indexes.
    populateTokenIndices();

    //boring printing stuff
    printTokenList();
    printSymTable();
    printSymTableInHashTableFormat();
    putTokenListInFile();
    fclose(yyin);
}

void init() {
    for ( int i = 0; i < 10; i++) {
        st[i] = NULL;    
    }
}

//Real Object Makers
token* newToken(char lex[], char tn[], int i, int l, int c) {
    token* temp = (token*)malloc(sizeof(token));
    strcpy(temp->tName, tn);
    strcpy(temp->lName, lex);
    temp->index = i;
    temp->line = l;
    temp->column = c;
    return temp;
}

symTableEntry* newSte(char l[]) {
    symTableEntry* temp = (symTableEntry*)malloc(sizeof(symTableEntry));
    temp->id = scnt + 1;
    temp->next = NULL;
    strcpy(temp->varName, l);
    return temp;
}


//Symbol Table Logic
void populateTokenIndices() {
    for ( int i = 0; i < tInd; i++ ) {
        
        //scope helper
        if ( TokenList[i]->lName[0] == LC || strcmp(TokenList[i]->tName, "LP") == 0 ) {
            localizer += 1;
        } else if ( TokenList[i]->lName[0] == RC || strcmp(TokenList[i]->tName, "RP") == 0) {
            localizer -= 1;
        }
        
        TokenList[i]->index = makeSTEForThisToken(TokenList[i], i);
        
    }
}

int makeSTEForThisToken(token* currToken, int i) {
    int ind, isFunc = 0;
    if ( currToken->index == -2 ) { //it is identifier AND not yet assigned before
        
        ind = search(st, currToken->lName);

        if( (ind == -1) ) { //entry dont exist as of yet
            symTableEntry* ste = newSte(currToken->lName);
            ste->scope = getScope();                                                    //scope                                                 
            if ( TokenList[i + 1]->lName[0] == '(' ) { //if func then this
                
                ste->scope = '-';
                strcpy(ste->varMemSize, "NULL");

                if (localizer == 0) {  //This means its func declaration                //return type
                    strcpy(ste->returnType, TokenList[i - 1]->lName);                   
                    int v = i + 1;
                    int argCnt = 0;
                    token* tempArgList[256];
                    while (strcmp(TokenList[v]->tName, "RP") != 0) { //if they are unequal then do this
                        if ( strcmp(TokenList[v]->tName, "IDENTIFIER") == 0) {
                            ste->args[argCnt] = TokenList[v];
                        }
                        if ( TokenList[v]->lName[0] == ',' ) {
                            argCnt += 1;
                        }
                        
                        v += 1;
                    }
                    if ( argCnt != 0 ) argCnt += 1;
                    ste->numArgs = argCnt;

                } else {
                    strcpy(ste->returnType, "NULL");
                }
                strcpy(ste->varType, "FUNCTION");
            
            } else { //if var then this
                
                //get VAR TYPE   
                if (TokenList[i - 1]->lName[0] == ',' || TokenList[i - 1]->lName[0] == '*') { //if delimiter or pointer?, then keep going back
                    int tv = i - 1;
                    while(strcmp(TokenList[tv]->tName, "KEYWORD") != 0 ) {
                        tv -= 1;
                    }
                    strcpy(ste->varType, TokenList[tv]->lName);
                } else {
                    strcpy(ste->varType, TokenList[i - 1]->lName);
                }


                char temp[256] = "sizeof(";
                strcat(temp, ste->varType);
                strcat(temp, ")");
                if ( strcmp(TokenList[i + 1]->tName, "LSB") == 0 ) {    
                    if ( strcmp(TokenList[i + 2]->tName, "NUMBER") == 0 ){
                        strcat(temp, " * ");                        
                        strcat(temp, TokenList[i + 2]->lName);
                    }
                } 
                strcpy(ste->varMemSize, temp);
                ste->numArgs = 0;
                strcpy(ste->returnType, "-");
            }

            insert(st, ste, &scnt);
            return ste->id;
        } else {
            return ind;
        }
    }
}

char getScope() {
    if ( localizer == 0 ) {
        return 'G';
    } else {
        return 'L';
    }
}


//STUPID PRINT FUNCS BELOW THIS
//--------------------------------------------------

void printTokenList() {
    printf("\n");
    printf("      LEX  |   TOKEN   | INDEX | LN | COL  \n");
    printf("-----------------------------------------------\n");
    for ( int i = 0; i < tInd; i++ ) {
        printf("<%10s %10s   %6d %3d %3d >\n", TokenList[i]->lName, TokenList[i]->tName, TokenList[i]->index, TokenList[i]->line, TokenList[i]->column);
    }
}

void printSymTable() {
    printf("\n");
    printf("---------------------------------------------------------------------------------------------------------------------\n");
    printf("|                                                    SYMBOL TABLE                                                    \n");
    printf("---------------------------------------------------------------------------------------------------------------------\n");
    printf("|    VAR    |    TYPE   | INDEX |       VAR_SIZE       | SCOPE| ARG_CNT| RET TYPE |         ARGUMENT LIST            \n");
    printf("------------+-----------+-------+----------------------+------+--------+----------+----------------------------------\n");
    symTableEntry* temp;
    for(int i = 0; i < 10; i++) {
        for(temp = st[i]; temp; temp = temp->next) {
            printf("|%10s | %9s | %5d |  %18s  |  %3c | %6d | %8s |", temp->varName, temp->varType, temp->id, temp->varMemSize, temp->scope, temp->numArgs, temp->returnType);
            if ( temp->numArgs > 0) {
                for ( int i = 0; i < temp->numArgs; i++) {
                    printf(" %8s ", temp->args[i]->lName);
                }
            }
            printf("\n");
        }
    }
    printf("=====================================================================================================================\n");
}

void printSymTableInHashTableFormat() {
    symTableEntry* temp;
    printf("\n");
    printf("SYMBOL TABLE AS HASH TABLE\n");
    printf("--------------------------\n");
    for(int i = 0; i < 10; i++) {
        printf("[%d] -> ", i);
        for(temp = st[i]; temp; temp = temp->next) {
            printf(" {%10s, %4d} ->", temp->varName, temp->id);
        }
        printf(" NULL\n");
    }
}

void putTokenListInFile() {
    FILE *o = fopen("tokens.txt", "w");
    for ( int i = 0; i < tInd; i++ ) {
        fprintf(o, "<%70s %10s %10d %10d %10d >\n", TokenList[i]->lName, TokenList[i]->tName, TokenList[i]->index, TokenList[i]->line, TokenList[i]->column);
    }
}
