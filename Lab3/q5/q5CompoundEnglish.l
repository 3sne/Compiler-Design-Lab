%{
typedef enum bool { false, true }bool;
bool isCompound = false;
%}

%%

" "+("and"|"or"|"because"|"cuz")" "+           {isCompound = true;}


%%

int main() {
    yylex();
    if ( isCompound == true ) {
        printf("\nisCompound\n");
    } else {
        printf("\nisNotCompund\n");
    }
}