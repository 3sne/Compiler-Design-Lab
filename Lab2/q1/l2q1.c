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

typedef struct token {
    char lname[256];
    int index; 
    unsigned int row,col;
    unsigned int type;
} token;

token* TokenList[1024];
char lex[256];
char buf[2048];
int lind = 0, bind = 0, line = 1, col = 1, cp, fp;

void popLex(char s[], char buf[], int cp, int fp) {
    int count = 0;
    for (int i = cp; i <= fp; i++ ) {
        s[count] = buf[i];
        count++;
    }
    s[count] = '\0';
}

int predict(char s[]) {
    //kw
    if ( isalpha(s[strlen(s) - 1]) ) {
        for ( int i = 0; i < 12; i++ ) {
            if ( strcmp(KW[i], s) == 0 ) {
                printf("%s\n", s);
                return 11;
            }
        }
        return 1;
    }
    return 0;
}

int main(int argc, char const *argv[]) {
    FILE *f;
    f = fopen( "in.c" , "r" );
    char c;

    int test = 0;
    do {
        lind = 0;
        c = getc(f);

        while ( c != '\n') { //put whole line in a large buffer
            buf[bind++] = c;
            c = getc(f);
        }
        buf[bind] = '\0';
        cp = fp = 0;
        popLex(lex, buf, cp, fp);
        while ( fp < bind && buf[fp] != '\0') {
            popLex(lex, buf, cp, fp);
            if ( (predict(lex) == 1) || (fp - cp == 0)) {
                fp += 1;
                
            } else if ( predict(lex) == 11 ) {
                cp = fp;
                
            } else {
                //fp--;
                //makeToken(lex);
                // printf("somedummytoken\n");
                fp++;
                cp = fp;
                
            }
        }
        test += 1;
    } while (test < 4);
    // } while (c != EOF);
    return 0;
}
