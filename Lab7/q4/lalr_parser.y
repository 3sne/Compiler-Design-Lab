%{
    #include <stdio.h>
    #include <stdlib.h>
    extern FILE *yyin;
    int yyerror(char *msg);
    int yylex();
%}

%token NL NUM ADD SUBTRACT MULTIPLY DIVIDE CAP N;

%%

input   : input line 
          | eps
          ;

line    : NL
          | exp NL
          ;

exp     : NUM
          | exp exp ADD
          | exp exp SUBTRACT
          | exp exp MULTIPLY
          | exp exp DIVIDE
          | exp exp CAP
          | exp N
          ;

eps     : 
          ;          

%%

int yyerror(char *msg) {
    printf("INVALID EXPR >> %s\n", msg);
}

void main() {
    yyin = fopen("input.txt", "r");
    (yyparse()) ? printf("VERDICT: REJECTED by grammar G\n") : printf("VERDICT: ACCEPTED by grammar G\n");
    fclose(yyin);
}