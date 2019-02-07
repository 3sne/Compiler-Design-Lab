%{
int vowelCount = 0;
int constCount = 0;
%}

%%

[a|e|i|o|u|A|E|I|O|U]       {vowelCount += 1;}
[a-zA-Z]                    {constCount += 1;}
%%

int main() {
    yylex();
    printf("VOWELS | CONSONANTS\n");
    printf("-------+-----------\n");
    printf("%6d | %9d|\n", vowelCount, constCount);
}