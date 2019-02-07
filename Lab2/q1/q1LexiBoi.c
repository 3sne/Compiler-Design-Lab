#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Ds
typedef struct token {
    char lexemeName[256];
    char tokenName[256];
    char attribute[256];
}token;

typedef struct symbolTableEntry {
    int attribute;
    char tokenName[256];
}symbolTableEntry;

char keyword[][256] = {
    "int",
    "char",
    "void",
    "if",
    "for",
    "while",
    "do",
    "break",
    "continue"
};
symbolTableEntry* symbolTable[256];
token* tokenList[1024];
int gAttr = 0, tInd = 0;

//Algo
int isArith(char c) {
    if ( c == '+' ||c == '*' ||c == '-' ||c == '/') {
        return 1;
    }
    return 0;
}

int isComp( char s[] ) {
   if (strcmp(s, "<") == 0 || strcmp(s, ">") == 0 || strcmp(s, "!") == 0 ) {
       return 1;
   }
   if (strcmp(s, "<=") == 0 || strcmp(s, ">=") == 0 || strcmp(s, "==") == 0 || strcmp(s, "!=")) {
       return 2;
   }
   return 0;
}

int isLogic( char s[] ) {
    if (strcmp(s, "&") == 0 || strcmp(s, "|") == 0) {
        return 1;
    }
    if ( strcmp(s, "&&") == 0 || strcmp(s, "||") == 0) {
        return 2;
    }
    return 0;
}

char* getTokenName(char s[]) {

}

void updateSymbolTable(int i, char s[]) {
    symbolTable[i]->attribute = i;
    strcpy(symbolTable[i]->tokenName, s);
}

token* generateNewToken(char s[]) {
    //printf("%s", s);
    int attr;
    token* temp;
    temp = (token*)malloc(sizeof(token));
    strcpy(temp->lexemeName, s);
    strcpy(temp->tokenName, getTokenName(s));
    if ( strcmp(temp->tokenName, "ID") == 0 ) {
        attr = gAttr;
        gAttr += 1;
        sprintf(temp->attribute, "%d", attr);
        updateSymbolTable(attr, s);
    } else {
        strcpy(temp->attribute, s);
    }

    //!
    tokenList[tInd] = temp;
    printf("%s %s\n", tokenList[tInd]->lexemeName, tokenList[tInd]->attribute);
    tInd += 1;
    
    return temp;
}

char getNextToken() {
    //this function should exist inside SA. But here we only care about LA. do-while loop in main is sort of this thing.
}

int main(int argc, char const *argv[])
{
    token *temp;
    FILE *f, *o;
    char lexeme[256] = "", helper[256], temper[256];
    char c;
    f = fopen("in.txt", "r");
    do {
        c = getc(f);
        if (c == '\t' || c == '\n') {
            generateNewToken(lexeme);
            lexeme[0] = '\0';
            continue;
        }

        if ( c != ' ') {
            temper[0] = c;
            temper[1] = '\0';
            if ( isArith(c) ) {
                lexeme[0] = c;
                lexeme[1] = '\0';
                generateNewToken(lexeme);
            }
            
            if ( isComp(temper) == 1) { //single match
                lexeme[0] = c;
                lexeme[1] = '\0';
                strcpy(helper, lexeme);
                c = getc(f);
                helper[strlen(helper)] = c;
                helper[strlen(helper)] = '\0';
                if (isComp(helper) == 2 ) { //double match
                    strcpy(lexeme, helper);
                    generateNewToken(lexeme);
                } else { //double mismatch
                    generateNewToken(lexeme);
                    continue;
                }
            }

            if ( isLogic(temper) == 1) { //single match
                lexeme[0] = c;
                lexeme[1] = '\0';
                strcpy(helper, lexeme);
                c = getc(f);
                helper[strlen(helper)] = c;
                helper[strlen(helper)] = '\0';
                if (isLogic(helper) == 2 ) { //double match
                    strcpy(lexeme, helper);
                    generateNewToken(lexeme);
                } else { //double mismatch
                    generateNewToken(lexeme);
                    continue;
                }
            }

            lexeme[strlen(lexeme)] = c;
            lexeme[strlen(lexeme)] = '\0';

        } else { //lexeme ends
            lexeme[strlen(lexeme)] = c;
            lexeme[strlen(lexeme)] = '\0';
            generateNewToken(lexeme);
            lexeme[0] = '\0';
        }
    } while(c != EOF);

    // for ( int i = 0; i < gAttr; i++ ) {
    //     printf("%d %11s\n", symbolTable[i]->attribute, symbolTable[i]->tokenName);
    // }

    for ( int i = 0; i < tInd; i++ ) {
        printf("<%s, %s>\n", tokenList[i]->attribute, tokenList[i]->lexemeName);
    }

    return 0;
}
