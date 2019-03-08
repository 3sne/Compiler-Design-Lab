%{
    #include <stdio.h>
    #include <stdlib.h>
    extern FILE * yyin;
    int yyerror(char *msg);
    int yylex();
%}

%token LITERAL SEMICOLON COMMA COLON ASSIGN LT GT LTE GTE EQUAL NOTEQUAL ADD MULTIPLY SUBTRACT DIVIDE COMP_AND COMP_DAND VOID COMP_OR COMP_DOR LP RP LC RC LSB RSB CHAR INT UINT SIGNED UNSIGNED SHORT LONG FLOAT DOUBLE REGISTER CONST IF ELSE FOR WHILE DO SWITCH CASE DEFAULT BREAK CONTINUE ENUM TYPEDEF EXTERN RETURN UNION GOTO ID NUM MOD;

%%

E       : T Eopt
          ;

Eopt    : ADD T Eopt 
          | SUBTRACT T Eopt
          | eps
          ;

T       : F Topt
          ;

Topt    : MULTIPLY F Topt 
          | DIVIDE F Topt 
          | eps
          ;

F       : NUM
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