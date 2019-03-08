%{
    #include <stdio.h>
    #include <stdlib.h>
    extern FILE * yyin;
    int yyerror(char *msg);
    int yylex();
%}

%token INT CHAR SBO SBC NL SEMICOL COMMA NUMBER ID;

%%

declarations    : data_type identifier_list SEMICOL declarations
                  | eps
                  ;

data_type       : INT
                  | CHAR
                  ;

identifier_list : ID
                  | ID COMMA identifier_list 
                  | ID SBO NUMBER SBC COMMA identifier_list
                  | ID SBO NUMBER SBC 
                  ;

eps             : 
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