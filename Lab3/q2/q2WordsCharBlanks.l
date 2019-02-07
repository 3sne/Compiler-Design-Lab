%{
int charCount = 0;
int blankCount = 0;
int wordCount = 0;
int lineCount = 0;
%}

%%

"\n"            {lineCount += 1;}
" "             {blankCount += 1;}
[a-zA-Z]+       {wordCount += 1; charCount += strlen(yytext);}

%%

int main() {
    yylex();
    printf("%10d %10d %10d %10d\n", charCount, blankCount, wordCount, lineCount);
}