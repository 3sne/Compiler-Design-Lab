%{
int posIntCount = 0;
int negIntCount = 0;
int posFloatCount = 0;
int negFloatCount = 0;
%}

%%

"+"?[0-9]+                  {posIntCount += 1;}
"-"[0-9]+                   {negIntCount += 1;}
"+"?[0-9]+"."[0-9]+         {posFloatCount += 1;}
"-"[0-9]+"."[0-9]+          {negFloatCount += 1;}

%%

int main() {
    yyin = fopen("input.txt", "r");
    yylex();
    FILE *f = fopen("input.txt", "r");
    printf("\nInput (change input.txt for variations): \n");
    char c;
    do {
        c = getc(f);
        printf("%c", c);
    } while (c != EOF);
    printf("\n%5d %5d %5d %5d\n", posIntCount, negIntCount, posFloatCount, negFloatCount);
}

int yywrap() {
    return 1;
}