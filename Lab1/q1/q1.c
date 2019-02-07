#include <stdio.h>

int main() {
    
    FILE *f, *o;
    int c, n;

    f = fopen("in.txt", "r");
    o = fopen("out.txt", "w");
    c = getc(f);

    if ( f == NULL ) {
        printf("input file error\n");
        return 0;
    }

    while ( c != EOF ) {
        if ( c == ' ' || c == '\t') {
            n = getc(f);
            if ( n == ' ' || n == '\t') {
                while ( c == ' ' || c == '\t') {
                    c = getc(f);
                }
                putc(' ', o);
                putc(c, o);
            } else {
                if ( c == '\t' ) {
                    putc(' ', o);
                } else {
                    putc(c, o);
                }
                putc(n, o);
            }
        } else {
            putc(c, o);
        }
        c = getc(f);
    }

}












































// #include <stdio.h>

// int main() {
    
//     FILE *f, *o;
//     int c, n;

//     f = fopen("in.txt", "r");
//     o = fopen("out.txt", "w");
//     c = getc(f);

//     if ( f == NULL ) {
//         printf("input file error\n");
//         return 0;
//     }

//     while ( c != EOF ) {
//         if ( c == ' ') {
//             n = getc(f);
//             if ( n == ' ' || n == '\t') {
//                 while ( c == ' ' || c == '\t') {
//                     c = getc(f);
//                 }
//             } else {
//                 putc(c, o);
//                 putc(n, o);
//                 c = getc(f);
//                 continue;
//             }
//         } else {
//             putc(c, o);
//             c = getc(f);
//             continue;
//         }
//         putc(' ', o);
//         putc(c, o);
//         c = getc(f);
//     }

// }