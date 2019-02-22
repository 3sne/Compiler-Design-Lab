/*
    @Author:        3sne ( Mukur Panchani )
    @FileName:      q4rdp.c
    @Task:          RDP for given grammar G
    @Dependency:    token.h tokenizer.exe input.txt

    G:
        lexp     -> aterm|list
        aterm    -> number|id
        list     -> (lexp_seq)
        lexp_seq -> lexp_seqlexp|lexp

    This grammar has left-recursion. RDP don't work well with left-recursive grammar since long strings may lead to long call stack.
    Ideally, ambiguity in first production can be removed via construction of predicitive parse table, but we skip it for this small example.
    Converting it to equivalent G',

    G':
        lexp     -> aterm|list
        aterm    -> number|id
        list     -> (lexp_seq)
        lexp_seq -> lexplexp_seq
        lexp_seq'-> lexplexp_seq'|<epsilon>
*/
#include "token.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int matchFlag = 1;
token* lookahead;

void lexp();
void aterm();
void list();
void lexp_seq();
void lexp_seqdash();

int main () {
    setupTokenList();
    printTokenList();
    lexp();
    lookahead = getNextToken(); //to confirm all tokens are acutally exhausted via EOTL. Otherwise, partial matches.
    if ( matchFlag && strcmp(lookahead->tName, "EOTL") == 0 ) {
        printf("Verdict: ACCEPTED.");
    } else {
        printf("Verdict: DENIED.");
    }
    return 0;
}

void lexp() {
    //this is weird case because we have option between two non-terminals, so we see their first() using getNextToken() to select path, then retract before commiting.
    //Could've been "normalized" by changing the grammar to remove ambiguity, but oh well too late now, or using predictive parsing table.
    lookahead = getNextToken();
    if (strcmp(lookahead->tName, "NUMBER") == 0 || strcmp(lookahead->tName, "IDENTIFIER") == 0 ) {
        retract();
        aterm();
        return;
    } else if (strcmp(lookahead->lName, "(") == 0) {
        retract();
        list();
        return;
    } else {
        matchFlag = 0;
        printf("e1\n");
    }
}

void aterm() {
    lookahead = getNextToken();
    if (strcmp(lookahead->tName, "NUMBER") == 0 ) {
        return;
    } else if ( strcmp(lookahead->tName, "IDENTIFIER") == 0 ) {
        return;
    } else {
        matchFlag = 0;
        printf("e2\n");
    }
}

void list() {
    lookahead= getNextToken();
    if (strcmp(lookahead->lName, "(") == 0) {
        lexp_seq();
        lookahead= getNextToken();
        if (strcmp(lookahead->lName, ")") == 0) {
            return;
        } else {
            matchFlag = 0;
            printf("e4\n");
        }
    } else {
        matchFlag = 0;
        printf("e3\n");
    }
}

void lexp_seq() {
    lexp();
    lexp_seqdash();
}

void lexp_seqdash() {
    lookahead = getNextToken();
    if (strcmp(lookahead->tName, "NUMBER") == 0 || strcmp(lookahead->tName, "IDENTIFIER") == 0 || strcmp(lookahead->lName, "(") == 0) {
        retract();
        lexp();
        lexp_seqdash();
        return;
    }
    retract();
}