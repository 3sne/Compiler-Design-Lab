%{
    #include <stdio.h>
    #include <stdlib.h>
    extern FILE *yyin;
    int yyerror(char *msg);
    int yylex();
%}

%token LITERAL SEMICOLON COMMA COLON ASSIGN LT GT LTE GTE EQUAL NOTEQUAL ADD MULTIPLY SUBTRACT DIVIDE MOD COMP_AND COMP_DAND COMP_OR COMP_DOR LP RP LC RC LSB RSB VOID INT UINT SIGNED UNSIGNED SHORT LONG FLOAT DOUBLE CHAR REGISTER CONST IF ELSE FOR WHILE DO SWITCH CASE DEFAULT BREAK CONTINUE ENUM TYPEDEF EXTERN RETURN UNION GOTO ID NUM MAIN;
%%

program         :   data_type MAIN LP RP LC declarations statement_list RC
                    ;

declarations    :   data_type identifier_list SEMICOLON declarations 
                    | eps
                    ;

data_type       :   INT 
                    | CHAR
                    ;

identifier_list :   ID 
                    | ID COMMA identifier_list 
                    | ID LSB NUM RSB COMMA identifier_list
                    | ID LSB NUM RSB 
                    ;

SS              :   SWITCH LP  expn RP LC  LSS  RC
                    ;

LSS             :   label_stmt LSS 
                    | eps
                    ;

label_stmt      :   CASE NUM COLON statement_list
                    | DEFAULT COLON statement_list
                    ;

looping_stat    :   WHILE LP expn RP LC statement_list RC 
                    | FOR LP assign_stat SEMICOLON expn SEMICOLON assign_stat RP LC statement_list RC
                    ;

expn            :   simple_expn eprime
                    ;

simple_expn     :   term seprime
                    ;

eprime          :   relop simple_expn 
                    | eps
                    ;

term            :   factor tprime
                    ;

factor          :   ID 
                    | NUM
                    ;

tprime          :   mulop factor tprime 
                    | eps
                    ;

mulop           :   MULTIPLY 
                    | DIVIDE 
                    | MOD
                    ;

seprime         :   addop term seprime 
                    | eps
                    ;

addop           :   ADD
                    | SUBTRACT
                    ;

relop           :   EQUAL 
                    | NOTEQUAL 
                    | LTE 
                    | GTE 
                    | GT 
                    | LT
                    ;

statement_list  :   statement statement_list 
                    | eps
                    ;

statement       :   assign_stat SEMICOLON 
                    | decision_stat 
                    | looping_stat 
                    | SS 
                    | BREAK SEMICOLON 
                    | CONTINUE SEMICOLON
                    ;

assign_stat     :   ID ASSIGN expn
                    ;

decision_stat   :   IF LP expn RP LC statement_list RC dprime
                    ;

dprime          :   ELSE LC statement_list RC 
                    | eps
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