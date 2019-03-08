# Q1 = $(shell pwd)/q1

parser:	test.tab.c lex.yy.c
	gcc test.tab.c lex.yy.c -o parser

test.tab.c:	test.y
	bison -d test.y

lex.yy.c: tokenizer.l test.tab.h
	flex tokenizer.l

clean:
	rm *.c *.h
