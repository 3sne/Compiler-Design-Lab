####LOCATIONS--------------------------------------------------------
q1 = $(shell pwd)/q1/
q2 = $(shell pwd)/q2/
q3 = $(shell pwd)/q3/
q4 = $(shell pwd)/q4/
#####################################################################


####RUN_ALL----------------------------------------------------------
all: parser1 parser2 parser3 parser4								
#####################################################################


####COMPILE----------------------------------------------------------
parser1: $(q1)lalr_parser.tab.c $(q1)lex.yy.c
	gcc $(q1)lalr_parser.tab.c $(q1)lex.yy.c -o $(q1)parser

parser2: $(q2)lalr_parser.tab.c $(q2)lex.yy.c
	gcc $(q2)lalr_parser.tab.c $(q2)lex.yy.c -o $(q2)parser

parser3: $(q3)lalr_parser.tab.c $(q3)lex.yy.c
	gcc $(q3)lalr_parser.tab.c $(q3)lex.yy.c -o $(q3)parser

parser4: $(q4)lalr_parser.tab.c $(q4)lex.yy.c
	gcc $(q4)lalr_parser.tab.c $(q4)lex.yy.c -o $(q4)parser
#####################################################################	


####PRE-COMPILE------------------------------------------------------
#Q1
$(q1)lalr_parser.tab.c: $(q1)lalr_parser.y
	bison -d $(q1)lalr_parser.y
	mv lalr_parser.tab.* $(q1)

$(q1)lex.yy.c: tokenizer.l $(q1)lalr_parser.tab.h
	flex tokenizer.l
	mv lex.yy.c $(q1)

#Q2
$(q2)lalr_parser.tab.c: $(q2)lalr_parser.y
	bison -d $(q2)lalr_parser.y
	mv lalr_parser.tab.* $(q2)

$(q2)lex.yy.c: tokenizer.l $(q2)lalr_parser.tab.h
	flex tokenizer.l
	mv lex.yy.c $(q2)

#Q3
$(q3)lalr_parser.tab.c: $(q3)lalr_parser.y
	bison -d $(q3)lalr_parser.y
	mv lalr_parser.tab.* $(q3)

$(q3)lex.yy.c: tokenizer.l $(q3)lalr_parser.tab.h
	flex tokenizer.l
	mv lex.yy.c $(q3)


#Q4
$(q4)lalr_parser.tab.c: $(q4)lalr_parser.y
	bison -d $(q4)lalr_parser.y
	mv lalr_parser.tab.* $(q4)

$(q4)lex.yy.c: $(q4)tokenizer.l $(q4)lalr_parser.tab.h
	flex $(q4)tokenizer.l
	mv lex.yy.c $(q4)
#####################################################################


####CLEAN-UP---------------------------------------------------------
clean:
	rm $(q1)*.c $(q1)*.h
	rm $(q2)*.c $(q2)*.h
	rm $(q3)*.c $(q3)*.h
	rm $(q4)*.c $(q4)*.h

clean-publish:
	rm $(q1)parser
	rm $(q2)parser
	rm $(q3)parser
	rm $(q4)parser
#####################################################################