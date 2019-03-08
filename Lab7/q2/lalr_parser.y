%{
    #include <stdio.h>
    #include <stdlib.h>
    extern FILE * yyin;
    int yyerror(char *msg);
    int yylex();
%}

%token LITERAL SEMICOLON COMMA COLON ASSIGN LT GT LTE GTE EQUAL NOTEQUAL ADD MULTIPLY SUBTRACT DIVIDE COMP_AND COMP_DAND VOID COMP_OR COMP_DOR LP RP LC RC LSB RSB CHAR INT UINT SIGNED UNSIGNED SHORT LONG FLOAT DOUBLE REGISTER CONST IF ELSE FOR WHILE DO SWITCH CASE DEFAULT BREAK CONTINUE ENUM TYPEDEF EXTERN RETURN UNION GOTO ID NUM MOD;

%%

decision_stat   : IF LP expn RP LC statement_list RC dprime
                  ;

expn            : simple_expn eprime
                  ;

statement_list  : statement statement_list 
                  | eps
                  ;

dprime          : ELSE LC statement_list RC 
                  | eps
                  ;

simple_expn     : term seprime
                  ;

eprime          : relop simple_expn 
                  | eps
                  ;

statement       : assign_stat SEMICOLON 
                  | decision_stat
                  ;

term            : factor tprime
                  ;

seprime         : addop term seprime 
                  | eps
                  ;

relop           : EQUAL
                  | NOTEQUAL
                  | LTE
                  | GTE
                  | GT
                  | LT
                  ;

assign_stat     : ID ASSIGN expn
                  ;

factor          : ID 
                  | NUM
                  ;

tprime          : mulop factor tprime 
                  | eps
                  ;

addop           : ADD 
                  | SUBTRACT
                  ;

mulop           : MULTIPLY 
                  | DIVIDE 
                  | MOD
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