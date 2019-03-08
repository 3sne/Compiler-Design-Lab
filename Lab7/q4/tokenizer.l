%{
    #include "lalr_parser.tab.h"
%}

%%

" "                         {                   }    
"\n"                        { return NL;        }
"+"                         { return ADD;       }
"*"                         { return MULTIPLY;  }
"-"                         { return SUBTRACT;  }
"/"                         { return DIVIDE;    }
"^"                         { return CAP;       }
"n"                         { return N;         }
[0-9]+("."?[0-9]+)?         { return NUM;       }

%%

int yywrap() {
    printf("Exec yywrap()\n");
    return 1;
}