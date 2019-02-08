Usage:
-> No need to compile flex file without changing it. Just use precompiled tokenizer.exe for all questions. If it is changed, just replace new compiled tokenizer.exe in each folder.
-> for any new input in input.txt, first run `./tokenizer`, then `./a.out` (i.e., the output of Grammar code)
-> This is how it works:
                                                   ------------------------------
    input.c (contain string to be checked by G) -> | lex.yy.c (from L3Q6 / L4Q1)| -> tokens.txt (input is tokenized) 
                                                   ------------------------------
                  -------------
    tokens.txt -> |thisProgram| -> Success/Failure
                  -------------
