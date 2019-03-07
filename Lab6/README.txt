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


Implementation reference notes:
    if epsilon is an option, that function will retract if not returned ( `return` only when accepts )
    if epsilon is not an option, error on mismatch.
    if production has atleast 1 terminal, do lookahead in the function, else no need.
    good practice to manually `return` when accept completes in a function
