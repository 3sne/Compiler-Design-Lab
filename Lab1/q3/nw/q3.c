#include <stdio.h>
#include <string.h>
#include <ctype.h>

char k[][256] = {
    "int",
    "char",
    "break",
    "continue",
    "for",
    "if",
    "else",
    "switch",
    "while",
    "do"
};

char buf[256];
int lineNo = 0, colNo = 0;

void checkBufContentsForKeyword(int bufi, int tempColNo) {
    for (int i = 0; i < 10; i++ ) {
        if ( strcmp(buf, k[i]) == 0) {
            printf("%s : [ %d : %d ]\n", k[i], lineNo, tempColNo - 1);
        }
    }
}


int main() {

    FILE *f, *o;
    int c, n;
    int bufi = 0, tempCol;

    f = fopen("in.c", "r");
    
    if ( f == NULL ) {
        printf("input file error\n");
        return 0;
    }

    c = getc(f);
    if (c == '\n') {
        lineNo += 1;
        colNo = 0;
    }

    while ( c != EOF ) {
        bufi = 0;
        if ( isalpha(c) ) { 
            buf[bufi++] = c;
            c = getc(f);
            colNo += 1;
            tempCol = colNo;
            while ( isalpha(c) ) {
                buf[bufi++] = c;
                c = getc(f);
                colNo += 1;
            }
            checkBufContentsForKeyword(bufi, tempCol);
        }
        c = getc(f);
        colNo += 1;
        if (c == '\n') {
            lineNo += 1;
            colNo = 0;
        }
    }
}





















    // while ( c != EOF ) {
    //     if ( isalpha(c) ) { 
    //         buf[++bufi] = c;
    //         n = getc(f);
    //         if ( isalpha(n) ) {
    //             while ( isalpha(n) ) {
    //                 buf[++bufi] = n;
    //                 n = getc(f);
    //             } 
    //         }
    //     }
    // }