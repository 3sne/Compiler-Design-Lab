%{
    #include "test.tab.h"
%}

%%

"int"   {return INT;}
"char"  {return CHAR;}
"["     {return SBO;}
"]"     {return SBC;}
"\n"    {return NL;}
","     {return COMMA;}
";"     {return SEMICOL;}
[0-9]+  {return NUMBER;}
[a-zA-Z][a-zA-Z0-9_]* {return ID;}



%%

int yywrap() {
    printf("\nWrapper Donzo\n");
    return 1;
}
