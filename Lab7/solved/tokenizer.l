%{
    #include "test.tab.h"
%}

%%

[0-9]+  {return NUMBER;}
"\n"    {return NL;}
[a-zA-Z][a-zA-Z0-9_]* {return ID;}
"." {return yytext[0];}
"+" {return ADD;}
"*" {return MUL;}

%%

int yywrap() {
    printf("Wrapper Donzo\n");
    return 1;
}
