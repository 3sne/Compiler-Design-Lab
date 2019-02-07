#include <stdio.h>

int main() {
    
    FILE *f, *o;
    int c, n;
    ///home/student/code/inputs/test1.c
    ///home/student/code/outputs/test1.c
    f = fopen("in.c", "r");
    o = fopen("out.c", "w");
    c = getc(f);

    if ( f == NULL ) {
        printf("input file error\n");
        return 0;
    }

    printf("Start reading...\n");
    while(c != EOF) {
        if ( c == '/') {
            n = getc(f);
            if (n == '/') {
                while(c != '\n') {
                    c = getc(f);
                }
            } else if (n == '*') {
                do {
                    while ( c != '*') {
                        c = getc(f);
                    }
                    c = getc(f);
                } while (c != '/');
            } else {
                putc(c, o);
                putc(n, o);
            }
        } else {
            putc(c, o);
        }
        c = getc(f);
    }
    printf("Done.\n");
    fclose(f);
    fclose(o);
}