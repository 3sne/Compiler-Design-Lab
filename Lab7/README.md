# Parsing strings with Bison

Bison constructs a LALR parser that is capable of accepting or rejecting strings based on the provided grammar. Coupled with Lex, it can be used as follows:

**Files**: `f.l` , `b.y` , `input.txt`

| Input     | Process           |     Output    |
| ----------|:-----------------:| --------------:|
| b.y       | Bison             | b.tab.c + b.tab.h|
| f.l (includes b.tab.h)      | Flex              | lex.yy.c       |
| b.tab.c + lex.yy.c       | GCC             | parser-executable|

The generated parser-executable, in this example, lies within the corresponding directory, the input to which is read from `input.txt`

These compiling tasks are tedious during debugging, so use following commands.

## To compile the source code for this lab, run:
```sh
$ make
```
Subsequently, clean the directories by running:
```sh
$ make clean
```
Before publishing new src changes, run:
```sh
$ make clean-publish
```
