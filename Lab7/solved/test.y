%{
    #include <stdio.h>
    #include <stdlib.h>
    extern FILE * yyin;
    int yyerror(char *msg);
    int yylex();
%}

%token NUMBER ID NL MUL ADD

%%

stmt:   exp
        ;

exp:    exp ADD term
        | term
        ;

term:   term MUL factor
        | factor
        ;

factor: ID
        | NUMBER
        ;

%%

int yyerror(char *msg) {
    printf("INVALID EXPR >> %s\n", msg);
}

void main() {
    yyin = fopen("in.txt", "r");
    if (yyparse() == 1) {
        printf("VERDICT: FAILURE\n");
    } else {
        printf("VERDICT: SUCCESS\n");
    }
}