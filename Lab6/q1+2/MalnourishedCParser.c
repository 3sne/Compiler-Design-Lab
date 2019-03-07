/*
    @Author:        3sne ( Mukur Panchani )
    @FileName:      MalnourishedCParser.c
    @Task:          Parses a very simple C program. Implemented as RDP using grammar G.
    @Dependency:    token.h tokenizer.exe input.txt
    @To-Do:         [ ] Parse break statements
                    [ ] Error recovery

    G:
        program         :=   data_type main () { declarations statement_list }
        declarations    :=   data_type identifier_list; declarations | <epsilon>
        data_type       :=   int | char
        identifier_list :=   id | id,identifier_list | id[num], identifier_list| id[num]
        SS              :=   switch ( expn ) { LSS }
        LSS             :=   label_stmt LSS | <epsilon>
        label_stmt      :=   case num: statement_list
                           | default: statement_list
        looping_stat    :=   while (expn) {statement_list} 
                           | for (assign_stat ; expn ; assign_stat ) {statement_list}
        expn            :=   simple_expn eprime
        simple_expn     :=   term seprime
        eprime          :=   relop simple_expn | <epsilon>
        term            :=   factor tprime
        factor          :=   id | num
        tprime          :=   mulop factor tprime | <epsilon>
        mulop           :=   * | / | %
        seprime         :=   addop term seprime | <epsilon>
        addop           :=   + | -
        relop           :=   ==|!=|<=|>=|>|<
        statement_list  :=   statement statement_list | <epsilon>
        statement       :=   assign_stat; | decision_stat | looping_stat | SS
        assign_stat     :=   id = expn
        decision_stat   :=   if ( expn ) {statement_list} dprime
        dprime          :=   else {statement_list} | <epsilon>
*/


//directives
#include "token.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


//Globals
int matchFlag = 1;
token* lookahead;
int DEBUGMODE = 0;


//Prototypes
void reportError(int l_no, token* tok, char *errMsg);
void program();
void declarations();
void data_type();
void identifier_list();
void SS();
void LSS();
void label_stmt();
void looping_stat();
void expn();
void simple_expn();
void eprime();
void term();
void factor();
void tprime();
void mulop();
void seprime();
void addop();
void relop();
void statement_list();
void statement();
void assign_stat();
void decision_stat();
void dprime();


//main
int main () {
    printf("Run in DEBUG MODE? Yes(1) | No(0) >> ");
    scanf("%d", &DEBUGMODE);
    setupTokenList();
    if (DEBUGMODE) printTokenList();
    program();
    lookahead = getNextToken(); //to confirm all tokens are acutally exhausted via EOTL($). Otherwise, partial matches.
    if ( matchFlag && strcmp(lookahead->tName, "EOTL") == 0 ) {
        printf("Verdict: ACCEPTED.");
    } else {
        printf("Verdict: DENIED.");
    }
    return 0;
}

/* 
    RDP production functions below this.
 */
void program() {
    data_type();
    lookahead = getNextToken();
    if(strcmp(lookahead->lName, "main") != 0 ) {
        reportError( __LINE__, lookahead, "Expected `main`");
        matchFlag = 0;
    }
    lookahead = getNextToken();
    if ( strcmp(lookahead->lName, "(") != 0 ) {
        reportError( __LINE__, lookahead, "Expected '('");
        matchFlag = 0;
    }
    lookahead = getNextToken();
    if ( strcmp(lookahead->lName, ")") != 0 ) {
        reportError( __LINE__, lookahead, "Expected ')'");
        matchFlag = 0;
    }
    lookahead = getNextToken();
    if ( strcmp(lookahead->lName, "{") != 0 ) {
        reportError( __LINE__, lookahead, "Expected '{'");
        matchFlag = 0;
    }
    declarations();
    statement_list();
    lookahead = getNextToken();
    if ( strcmp(lookahead->lName, "}") != 0 ) {
        reportError( __LINE__, lookahead, "Expected '}'");
        matchFlag = 0;
    }
}

void declarations() {
    lookahead = getNextToken();
    if ( strcmp(lookahead->lName, "int") == 0 || strcmp(lookahead->lName, "char") == 0) {
        retract();
        data_type();
        identifier_list();
        lookahead = getNextToken();
        if ( strcmp(lookahead->lName, ";") != 0 ) {
            reportError( __LINE__, lookahead, "Expected ';'");
            matchFlag = 0;
        }
        declarations();
        return;
    }
    retract();
}

void data_type() {
    lookahead = getNextToken();
    if (strcmp(lookahead->lName, "int") != 0 && strcmp(lookahead->lName, "char") != 0) {
        reportError( __LINE__, lookahead, "Expected data-type");
        matchFlag = 0;
    }
}
void identifier_list() {
    lookahead = getNextToken();
    if ( strcmp(lookahead->tName, "IDENTIFIER") != 0) {
        matchFlag = 0;
        reportError( __LINE__, lookahead, "Expected IDENTIFIER");
    }
    lookahead = getNextToken();
    if (strcmp(lookahead->lName, ",") == 0) {
        identifier_list();
    } else if (strcmp(lookahead->lName, "[") == 0) {
        lookahead = getNextToken();
        if ( strcmp(lookahead->tName, "NUMBER") != 0 ) {
            matchFlag = 0;
            reportError( __LINE__, lookahead, "Expected NUMBER");
        }
        lookahead = getNextToken();
        if( strcmp(lookahead->lName, "]") != 0) {
            matchFlag = 0;
            reportError( __LINE__, lookahead, "Expected ']'");
        }
        lookahead = getNextToken();
        if( strcmp(lookahead->lName, ",") == 0) {
            identifier_list();
            return;
        }
        retract();
    } else {
        retract();
    }
}

void SS() {
    lookahead = getNextToken();
    if (strcmp(lookahead->lName, "switch") == 0 ) {
        lookahead = getNextToken();
        if (strcmp(lookahead->lName, "(") == 0 ) {
            expn();
            lookahead = getNextToken();
            if (strcmp(lookahead->lName, ")") == 0 ) {
                lookahead = getNextToken();
                if (strcmp(lookahead->lName, "{") == 0 ) {
                    LSS();
                    lookahead = getNextToken();
                    if (strcmp(lookahead->lName, "}") == 0 ) {
                        return;
                    } else {
                        matchFlag = 0;
                        reportError( __LINE__, lookahead, "Expected '}'");
                        return;
                    }
                } else {
                    matchFlag = 0;
                    reportError( __LINE__, lookahead, "Expected '{'");
                    return;
                }
            } else {
                matchFlag = 0;
                reportError( __LINE__, lookahead, "Expected ')'");
                return;
            }
        } else {
            matchFlag = 0;
            reportError( __LINE__, lookahead, "Expected '('");
            return;
        }
    } else {
        matchFlag = 0;
        printf("e1\n");
        return;
    }
}

void LSS() {
    lookahead = getNextToken();
    if ( strcmp(lookahead->lName, "case") == 0 || strcmp(lookahead->lName, "default") == 0) {
        retract();
        label_stmt();
        LSS();
        return;
    }
    retract();
}

void label_stmt() {
    lookahead = getNextToken();
    if ( strcmp(lookahead->lName, "case") == 0 ) {
        lookahead = getNextToken();
        if ( strcmp(lookahead->tName, "NUMBER") == 0 ) {
            lookahead = getNextToken();
            if ( strcmp(lookahead->lName, ":") == 0 ) {
                statement_list();
                return;
            } else {
                matchFlag = 0;
                reportError( __LINE__, lookahead, "Expected ':'");
                return;
            }
        } else {
            matchFlag = 0;
            reportError( __LINE__, lookahead, "Expected NUMBER");
            return;
        }
    } else if (strcmp(lookahead->lName, "default") == 0) {
        lookahead = getNextToken();
        if ( strcmp(lookahead->lName, ":") == 0 ) {
            statement_list();
            return;
        } else {
            matchFlag = 0;
            reportError( __LINE__, lookahead, "Expected ':'");
            return;
        }
    } else {
        matchFlag = 0;
        reportError( __LINE__, lookahead, "Expected 'case' or 'default'");
        return;
    }
}

void looping_stat() {
    lookahead = getNextToken();
    if (strcmp(lookahead->lName, "while") == 0 ) {
        lookahead = getNextToken();
        if (strcmp(lookahead->lName, "(") != 0) {
            matchFlag = 0;
<<<<<<< HEAD:Lab6/q1+2/MalnourishedCParser.c
            reportError( __LINE__, lookahead, "Expected '('");
        }
        expn();
        lookahead = getNextToken();
        if (strcmp(lookahead->lName, ")") != 0) {
            matchFlag = 0;
            reportError( __LINE__, lookahead, "Expected ')'");
        }
        lookahead = getNextToken();
        if (strcmp(lookahead->lName, "{") != 0) {
            matchFlag = 0;
            reportError( __LINE__, lookahead, "Expected '{'");
        }
        statement_list();
        lookahead = getNextToken();
        if (strcmp(lookahead->lName, "}") != 0) {
            matchFlag = 0;
            reportError( __LINE__, lookahead, "Expected '}'");
        }
=======
            reportError(lookahead, "Expected '('");
        }
        expn();
        lookahead = getNextToken();
        if (strcmp(lookahead->lName, ")") != 0) {
            matchFlag = 0;
            reportError(lookahead, "Expected ')'");
        }
        lookahead = getNextToken();
        if (strcmp(lookahead->lName, "{") != 0) {
            matchFlag = 0;
            reportError(lookahead, "Expected '{'");
        }
        statement_list();
        lookahead = getNextToken();
        if (strcmp(lookahead->lName, "}") != 0) {
            matchFlag = 0;
            reportError(lookahead, "Expected '}'");
        }
>>>>>>> 324d9afc4f68107ebc836cca1ae82e406d1842e6:Lab6/q1/q1SwitchParser.c
        return;
    } else if (strcmp(lookahead->lName, "for") == 0) {
        lookahead = getNextToken();
        if (strcmp(lookahead->lName, "(") != 0 ) {
            matchFlag = 0;
<<<<<<< HEAD:Lab6/q1+2/MalnourishedCParser.c
            reportError( __LINE__, lookahead, "Expected '('");
        }
        assign_stat();
        lookahead = getNextToken();
        if (strcmp(lookahead->lName, ";") != 0 ) {
            matchFlag = 0;
            reportError( __LINE__, lookahead, "Expected ';'");
        }
        expn();
        lookahead = getNextToken();
        if (strcmp(lookahead->lName, ";") != 0 ) {
            matchFlag = 0;
            reportError( __LINE__, lookahead, "Expected ';'");
        }
        assign_stat();
        lookahead = getNextToken();
        if (strcmp(lookahead->lName, ")") != 0 ) {
            matchFlag = 0;
            reportError( __LINE__, lookahead, "Expected ')'");
        }
        lookahead = getNextToken();
        if (strcmp(lookahead->lName, "{") != 0 ) {
            matchFlag = 0;
            reportError( __LINE__, lookahead, "Expected '{'");
        }
=======
            reportError(lookahead, "Expected '('");
        }
        assign_stat();
        lookahead = getNextToken();
        if (strcmp(lookahead->lName, ";") != 0 ) {
            matchFlag = 0;
            reportError(lookahead, "Expected ';'");
        }
        expn();
        lookahead = getNextToken();
        if (strcmp(lookahead->lName, ";") != 0 ) {
            matchFlag = 0;
            reportError(lookahead, "Expected ';'");
        }
        assign_stat();
        lookahead = getNextToken();
        if (strcmp(lookahead->lName, ")") != 0 ) {
            matchFlag = 0;
            reportError(lookahead, "Expected ')'");
        }
        lookahead = getNextToken();
        if (strcmp(lookahead->lName, "{") != 0 ) {
            matchFlag = 0;
            reportError(lookahead, "Expected '{'");
        }
>>>>>>> 324d9afc4f68107ebc836cca1ae82e406d1842e6:Lab6/q1/q1SwitchParser.c
        statement_list();
        lookahead = getNextToken();
        if (strcmp(lookahead->lName, "}") != 0 ) {
            matchFlag = 0;
<<<<<<< HEAD:Lab6/q1+2/MalnourishedCParser.c
            reportError( __LINE__, lookahead, "Expected '}'");
=======
            reportError(lookahead, "Expected '}'");
>>>>>>> 324d9afc4f68107ebc836cca1ae82e406d1842e6:Lab6/q1/q1SwitchParser.c
        }
        return;
    } else {
        matchFlag = 0;
        printf("%s e2\n", lookahead->lName);
        return;
    }
}

void expn() {
    simple_expn();
    eprime();
}

void simple_expn() {
    term();
    seprime();
}

void eprime() {
    lookahead = getNextToken();
    if ( strcmp(lookahead->tName, "RLOP") == 0) {
        retract();
        relop();
        simple_expn();
        return;
    }
    retract();
}

void term() {
    factor();
    tprime();
}

void factor() {
    lookahead = getNextToken();
    if (strcmp(lookahead->tName, "IDENTIFIER") == 0 || strcmp(lookahead->tName, "NUMBER") == 0) {
        return;
    } else {
        matchFlag = 0;
        reportError( __LINE__, lookahead, "Expected IDENTIFIER OR NUMBER");
        return;
    }
}

void tprime() {
    lookahead = getNextToken();
    if (strcmp(lookahead->lName, "*") == 0 || strcmp(lookahead->lName, "/") == 0 ) {
        retract();
        mulop();
        factor();
        tprime();
        return;
    }
    retract();
}

void mulop() {
    lookahead = getNextToken();
    if (strcmp(lookahead->lName, "*") == 0 || strcmp(lookahead->lName, "/") == 0 ) {
        return;
    } else {
        matchFlag = 0;
        reportError( __LINE__, lookahead, "Expected '*' or '/'");
        return;
    }
}

void seprime() {
    lookahead = getNextToken();
    if (strcmp(lookahead->lName, "+") == 0 || strcmp(lookahead->lName, "-") == 0 ) {
        retract();
        addop();
        term();
        seprime();
        return;
    }
    retract();
}

void addop() {
    lookahead = getNextToken();
    if (strcmp(lookahead->lName, "+") == 0 || strcmp(lookahead->lName, "-") == 0 ) {
        return;
    } else {
        matchFlag = 0;
        reportError( __LINE__, lookahead, "Expected '+' or '-'");
        return;
    }
}

void relop() {
    lookahead = getNextToken();
    if ( strcmp(lookahead->tName, "RLOP") == 0) {
        return;
    } else {
        matchFlag = 0;
        reportError( __LINE__, lookahead, "Expected RELOP");
        return;
    }
}

void statement_list() {
    lookahead = getNextToken();
    if ( strcmp(lookahead->tName, "IDENTIFIER") == 0 || strcmp(lookahead->lName, "if") == 0 || strcmp(lookahead->lName, "for") == 0 || strcmp(lookahead->lName, "while") == 0 || strcmp(lookahead->lName, "switch") == 0) {
        retract();
        statement();
        statement_list();
        return;
    }
    retract();
}

void statement() {
    lookahead = getNextToken();
    if (strcmp(lookahead->tName, "IDENTIFIER") == 0) {
        retract();
        assign_stat();
        lookahead = getNextToken();
        if (strcmp(lookahead->lName, ";") == 0) {
            return;
        } else { //error missing ";"
            matchFlag = 0;
            reportError( __LINE__, lookahead, "Expected ';'");
            return;
        }
    } else if (strcmp(lookahead->lName, "if") == 0) {
        retract();
        decision_stat();
        return;
    } else if (strcmp(lookahead->lName, "while") == 0 || strcmp(lookahead->lName, "for") == 0) {
        retract();
        looping_stat();
        return;
    } else if (strcmp(lookahead->lName, "switch") == 0) {
        retract();
        SS();
        return;
    } else {
        matchFlag = 0;
        reportError( __LINE__, lookahead, "Unexpected input");
        return;
    }
}

void assign_stat() {
    lookahead = getNextToken();
    if ( strcmp(lookahead->tName, "IDENTIFIER") == 0 ) {
        lookahead = getNextToken();
        if ( strcmp(lookahead->lName, "=") == 0 ) {
            expn();
        } else {
            matchFlag = 0;
            reportError( __LINE__, lookahead, "Expected '='");            
            return;
        }
    } else {
        matchFlag = 0;
        reportError( __LINE__, lookahead, "Expected IDENTIFIER");
        return;
    }
}

void decision_stat() {
    lookahead = getNextToken();
    if ( strcmp(lookahead->lName, "if") == 0 ) {
        lookahead = getNextToken();
        if ( strcmp(lookahead->lName, "(") == 0 ) {
            expn();
            lookahead = getNextToken();
            if ( strcmp(lookahead->lName, ")") == 0 ) {
                lookahead = getNextToken();
                if ( strcmp(lookahead->lName, "{") == 0 ) {
                    statement_list();
                    lookahead = getNextToken();
                    if ( strcmp(lookahead->lName, "}") == 0 ) {
                        dprime();
                        return;
                    } else {
                        matchFlag = 0;
                        reportError( __LINE__, lookahead, "Expected '}'");
                        return;
                    }
                } else {
                    matchFlag = 0;
                    reportError( __LINE__, lookahead, "Expected '{'");
                    return;
                }
            } else {
                matchFlag = 0;
                reportError( __LINE__, lookahead, "Expected ')'");
                return;
            }
        } else {
            matchFlag = 0;
            reportError( __LINE__, lookahead, "Expected '('");
            return;
        }
    } else {
        matchFlag = 0;
        reportError( __LINE__, lookahead, "Expected 'if'");
        return;
    }
}

void dprime() {
    lookahead = getNextToken();
    if ( strcmp(lookahead->lName, "else") == 0 ) {
        lookahead = getNextToken();
        if ( strcmp(lookahead->lName, "{") == 0 ) {
            statement_list();
            lookahead = getNextToken();
            if ( strcmp(lookahead->lName, "}") == 0 ) {
                return;
            } else {
                matchFlag = 0;
                reportError( __LINE__, lookahead, "Expected '}'");
                return;
            }
        } else {
            matchFlag = 0;
            reportError( __LINE__, lookahead, "Expected '{'");
            return;
        }
        return;
    }
    retract();
}

// Error reporting / debug utility function
void reportError(int l_no, token* tok, char *errMsg) { //, int l_no, int debug) {
    (DEBUGMODE > 0) ? (DEBUGMODE = 1) : (DEBUGMODE = 0);
    char *finalStr = (char*)malloc(sizeof(char) * 1024);
    if (DEBUGMODE) {
        sprintf(finalStr, "[DEBUG: %d] [!] PARSE_ERROR @ {Line:Col}={%d:%d} >> %s | Got: '%s'", l_no, tok->line, tok->column, errMsg, tok->lName);
    } else {
        sprintf(finalStr, "[!ERROR!] PARSE_ERROR @ {Line:Col}={%d:%d} >> %s | Got: '%s'", tok->line, tok->column, errMsg, tok->lName);
    }
    fprintf(stderr, "%s\n", finalStr);
    free(finalStr);
}