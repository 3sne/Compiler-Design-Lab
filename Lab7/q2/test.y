%{
    #include <stdio.h>
    #include <stdlib.h>
    extern FILE * yyin;
    int yyerror(char *msg);
    int yylex();
%}

%token INT CHAR SBO SBC NL SEMICOL COMMA NUMBER ID;

%%

decision_stat   : if ( expn ) { statement_list } dprime
expn            : simple_expn eprime
statement_list  : statement statement_list | <epsilon>
dprime          : else {statement_list} | <epsilon>
simple_expn     : term seprime
eprime          : relop simple_expn | <epsilon>
statement       : assign_stat; | decision_stat
term            : factor tprime
seprime         : addop term seprime | <epsilon>
relop           : ==|!=|<=|>=|>|<
assign_stat     : id = expn
factor          : id | num
tprime          : mulop factor tprime | <epsilon>
addop           : + | -
mulop           : * | / | %

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