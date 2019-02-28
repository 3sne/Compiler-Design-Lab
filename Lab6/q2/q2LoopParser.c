/*
    @Author:        3sne ( Mukur Panchani )
    @FileName:      q2LoopParser.c
    @Task:          Parses loop statements in C. Implemented as RDP using grammar G.
    @Dependency:    token.h tokenizer.exe input.txt

    G:
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
        statement       :=   assign_stat; | decision_stat | looping_stat
        assign_stat     :=   id = expn
        decision_stat   :=   if ( expn ) {statement_list} dprime
        dprime          :=   else {statement_list} | <epsilon>
*/

#include "token.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int matchFlag = 1;
token* lookahead;

void reportError(token* tok, char *errMsg);
void looping_stat();    /* 1 */
void expn();            /* 2 */
void simple_expn();     /* 3 */
void eprime();          /* 4 */
void term();            /* 5 */
void factor();          /* 6 */
void tprime();          /* 7 */
void mulop();           /* 8 */
void seprime();         /* 9*/
void addop();           /* 10 */
void relop();           /* 11 */
void statement_list();  /* 12 */
void statement();       /* 13 */
void assign_stat();     /* 14 */
void decision_stat();   /* 15 */
void dprime();          /* 16 */


void reportError(token* tok, char *errMsg) {
    char *finalStr = (char*)malloc(sizeof(char) * 1024);
    char *lineNumber = (char*)malloc(sizeof(char) * 16);
    char *columnNumber = (char*)malloc(sizeof(char) * 16);
    sprintf(lineNumber, "%d", tok->line);
    sprintf(columnNumber, "%d", tok->column);
    strcpy(finalStr, "[!]PARSE_ERROR @ {Line:Col}={");
    strcat(finalStr, lineNumber);
    strcat(finalStr, ":");
    strcat(finalStr, columnNumber);
    strcat(finalStr, "} >> ");
    strcat(finalStr, errMsg);
    strcat(finalStr, " | Got: '");
    strcat(finalStr, tok->lName);
    strcat(finalStr, "'");
    fprintf(stderr, "%s\n", finalStr);
    free(finalStr);
    free(lineNumber);
    free(columnNumber);
}

int main () {
    setupTokenList();
    printTokenList();
    looping_stat();
    lookahead = getNextToken(); //to confirm all tokens are acutally exhausted via EOTL($). Otherwise, partial matches.
    if ( matchFlag && strcmp(lookahead->tName, "EOTL") == 0 ) {
        printf("Verdict: ACCEPTED.");
    } else {
        printf("Verdict: DENIED.");
    }
    return 0;
}

/* 1 */
void looping_stat() {
    lookahead = getNextToken();
    if (strcmp(lookahead->lName, "while") == 0 ) {
        lookahead = getNextToken();
        if (strcmp(lookahead->lName, "(") == 0) {
            expn();
            lookahead = getNextToken();
            if (strcmp(lookahead->lName, ")") == 0) {
                lookahead = getNextToken();
                if (strcmp(lookahead->lName, "{") == 0) {
                    statement_list();
                    lookahead = getNextToken();
                    if (strcmp(lookahead->lName, "}") == 0) {
                        return;
                    } else {
                        matchFlag = 0;
                        reportError(lookahead, "Expected '}'");
                        return;
                    }
                } else {
                    matchFlag = 0;
                    reportError(lookahead, "Expected '{'");
                    return;
                }
            } else {
                matchFlag = 0;
                reportError(lookahead, "Expected ')'");
                return;
            }
        } else {
            matchFlag = 0;
            reportError(lookahead, "Expected '('");
            return;
        }
    } else if (strcmp(lookahead->lName, "for") == 0) {
        lookahead = getNextToken();
        if (strcmp(lookahead->lName, "(") == 0 ) {
            assign_stat();
            lookahead = getNextToken();
            if (strcmp(lookahead->lName, ";") == 0 ) {
                expn();
                lookahead = getNextToken();
                if (strcmp(lookahead->lName, ";") == 0 ) {
                    assign_stat();
                    lookahead = getNextToken();
                    if (strcmp(lookahead->lName, ")") == 0 ) {
                        lookahead = getNextToken();
                        if (strcmp(lookahead->lName, "{") == 0 ) {
                            statement_list();
                            lookahead = getNextToken();
                            if (strcmp(lookahead->lName, "}") == 0 ) {
                                return;
                            } else {
                                matchFlag = 0;
                                reportError(lookahead, "Expected '}'");
                                return;
                            }
                        } else {
                            matchFlag = 0;
                            reportError(lookahead, "Expected '{'");
                            return;
                        }
                    } else {
                        matchFlag = 0;
                        reportError(lookahead, "Expected ')'");
                        return;
                    }
                } else {
                    matchFlag = 0;
                    reportError(lookahead, "Expected ';'");
                    return;
                }
            } else {
                matchFlag = 0;
                reportError(lookahead, "Expected ';'");
                return;
            }
        } else {
            matchFlag = 0;
            reportError(lookahead, "Expected '('");
            return;
        }
    }  else {
        matchFlag = 0;
        printf("e1\n");
        return;
    }
}

/* 2 */
void expn() {
    simple_expn();
    eprime();
}

/* 3 */
void simple_expn() {
    term();
    seprime();
}

/* 4 */
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

/* 5 */
void term() {
    factor();
    tprime();
}

/* 6 */
void factor() {
    lookahead = getNextToken();
    if (strcmp(lookahead->tName, "IDENTIFIER") == 0 || strcmp(lookahead->tName, "NUMBER") == 0) {
        return;
    } else {
        matchFlag = 0;
        reportError(lookahead, "Expected IDENTIFIER OR NUMBER");
        return;
    }
}

/* 7 */
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

/* 8 */
void mulop() {
    lookahead = getNextToken();
    if (strcmp(lookahead->lName, "*") == 0 || strcmp(lookahead->lName, "/") == 0 ) {
        return;
    } else {
        matchFlag = 0;
        reportError(lookahead, "Expected '*' or '/'");
        return;
    }
}

/* 9 */
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

/* 10 */
void addop() {
    lookahead = getNextToken();
    if (strcmp(lookahead->lName, "+") == 0 || strcmp(lookahead->lName, "-") == 0 ) {
        return;
    } else {
        matchFlag = 0;
        reportError(lookahead, "Expected '+' or '-'");
        return;
    }
}

/* 11 */
void relop() {
    lookahead = getNextToken();
    if ( strcmp(lookahead->tName, "RLOP") == 0) {
        return;
    } else {
        matchFlag = 0;
        reportError(lookahead, "Expected RELOP");
        return;
    }
}

/* 12 */
void statement_list() {
    lookahead = getNextToken();
    if ( strcmp(lookahead->tName, "IDENTIFIER") == 0 || strcmp(lookahead->lName, "if") == 0 || strcmp(lookahead->lName, "for") == 0 || strcmp(lookahead->lName, "while") == 0) {
        retract();
        statement();
        statement_list();
        return;
    }
    retract();
}

/* 13 */
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
            reportError(lookahead, "Expected ';'");
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
    } else {
        matchFlag = 0;
        reportError(lookahead, "Unexpected input");
        return;
    }
}

/* 14 */
void assign_stat() {
    lookahead = getNextToken();
    if ( strcmp(lookahead->tName, "IDENTIFIER") == 0 ) {
        lookahead = getNextToken();
        if ( strcmp(lookahead->lName, "=") == 0 ) {
            expn();
        } else {
            matchFlag = 0;
            reportError(lookahead, "Expected '='");            
            return;
        }
    } else {
        matchFlag = 0;
        reportError(lookahead, "Expected IDENTIFIER");
        return;
    }
}

/* 15 */
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
                        reportError(lookahead, "Expected '}'");
                        return;
                    }
                } else {
                    matchFlag = 0;
                    reportError(lookahead, "Expected '{'");
                    return;
                }
            } else {
                matchFlag = 0;
                reportError(lookahead, "Expected ')'");
                return;
            }
        } else {
            matchFlag = 0;
            reportError(lookahead, "Expected '('");
            return;
        }
    } else {
        matchFlag = 0;
        reportError(lookahead, "Expected 'if'");
        return;
    }
}

/* 16 */
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
                reportError(lookahead, "Expected '}'");
                return;
            }
        } else {
            matchFlag = 0;
            reportError(lookahead, "Expected '{'");
            return;
        }
        return;
    }
    retract();
}