#include <stdio.h>


int main() {

    FILE *f, *o;
    int c, n;

    f = fopen("in.c", "r");
    o = fopen("out.c", "w");
    c = getc(f);

    if ( f == NULL ) {
        printf("input file error\n");
        return 0;
    }

    while ( c != EOF ) {
        if ( c == '#' ) {
            while ( c != '\n') {
                c = getc(f);
            }
        } else {
            putc(c, o);
        }
        c = getc(f);
    }
}
