#include <stdio.h>
#include <string.h>
#include <ctype.h>

char KW [][256] = {
    "break",
    "case",
    "char",
    "continue",
    "do",
    "else",
    "for",
    "if",
    "int",
    "return",
    "switch",
    "while"
};

char lex[256];
int ind = 0;
int line = 1;
int col = 1;

int checkMatchandPrint(char s[]) {
    
    for ( int i = 0; i < 12; i++ ) {
        if ( strcmp(KW[i], s) == 0 ) {
            for (int j = 0; j < strlen(KW[i]); j++ ) {
                s[j] = toupper(KW[i][j]);
            }
            printf("%9s  %3d  %2d\n", s, line, col - strlen(KW[i]) - 1);
            return 1;
        }
    }
    return 0;
}

int main() {
    FILE *f;
    f = fopen( "in.c" , "r" );
    char c;

    if ( f == NULL ) {
        return 0;
    }

    printf("  KEYWORD  LINE  COL\n");
    printf("--------------------\n");
    do {
        c = getc(f);
        if ( c != ' ' && c != '\n' && c != '\t') {
            lex[ind++] = c;
            col += 1;
        } else {
            if ( c == ' ' ) {
                col += 1;
            } else if ( c == '\t' ) {
                col += 4;
            } else {
                line += 1;
                col = 1;
            }
            lex[ind] = '\0';
            checkMatchandPrint(lex);
            ind = 0;
        }

    } while (c != EOF);
}