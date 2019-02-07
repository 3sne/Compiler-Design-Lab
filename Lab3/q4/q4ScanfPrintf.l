%{
unsigned int nPrintf    = 0;
unsigned int nScanf     = 0;
%}

%%

"printf"        {nPrintf += 1; fprintf(yyout, "scanf");}
"scanf"         {nScanf += 1; fprintf(yyout, "printf");}

%%

int main() {
    yyin = fopen("input.c", "r");
    yyout = fopen("output.c", "w+");
    yylex();
    printf("%5d %5d", nPrintf, nScanf);
}