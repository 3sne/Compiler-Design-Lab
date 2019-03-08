%{
    #include <stdio.h>
    #include <stdlib.h>
    extern FILE * yyin;
    int yyerror(char *msg);
    int yylex();
%}

%token LITERAL SEMICOLON COMMA COLON ASSIGN LT GT LTE GTE EQUAL NOTEQUAL ADD MULTIPLY SUBTRACT DIVIDE MOD COMP_AND COMP_DAND VOID COMP_OR COMP_DOR LP RP LC RC LSB RSB CHAR INT UINT SIGNED UNSIGNED SHORT LONG FLOAT DOUBLE REGISTER CONST IF ELSE FOR WHILE DO SWITCH CASE DEFAULT BREAK CONTINUE ENUM TYPEDEF EXTERN RETURN UNION GOTO ID NUM;

%%

declarations    : data_type identifier_list SEMICOLON declarations
                  | eps
                  ;

data_type       : INT
                  | CHAR
                  ;

identifier_list : ID
                  | ID COMMA identifier_list 
                  | ID LSB NUM RSB COMMA identifier_list
                  | ID LSB NUM RSB 
                  ;

eps             : 
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
