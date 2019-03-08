%{
    #include "lalr_parser.tab.h"
%}

%%

"\t"|" "|"\n"               {                    }
(("#".*)|("//".*))"\n"      {                    }
("/*"(.|"\n")*"*/")         {                    }
"\"".*"\""                  { return LITERAL;    }
";"                         { return SEMICOLON;  }
","                         { return COMMA;      }
":"                         { return COLON;      }
"="                         { return ASSIGN;     }
"<"                         { return LT;         }
">"                         { return GT;         }
"<="                        { return LTE;        }
">="                        { return GTE;        }
"=="                        { return EQUAL;      }
"!="                        { return NOTEQUAL;   }
"+"                         { return ADD;        }
"*"                         { return MULTIPLY;   }
"-"                         { return SUBTRACT;   }
"/"                         { return DIVIDE;     }
"%"                         { return MOD;        }
"&"                         { return COMP_AND;   }
"&&"                        { return COMP_DAND;  }
"|"                         { return COMP_OR;    }
"||"                        { return COMP_DOR;   }
"("                         { return LP;         }
")"                         { return RP;         }
"{"                         { return LC;         }
"}"                         { return RC;         }
"["                         { return LSB;        }
"]"                         { return RSB;        }
"void"                      { return VOID;       }
"int"                       { return INT;        }
"uint"                      { return UINT;       }
"signed"                    { return SIGNED;     }
"unsigned"                  { return UNSIGNED;   }
"short"                     { return SHORT;      }
"long"                      { return LONG;       }
"float"                     { return FLOAT;      }
"double"                    { return DOUBLE;     }
"char"                      { return CHAR;       }
"register"                  { return REGISTER;   }
"const"                     { return CONST;      }
"if"                        { return IF;         }
"else"                      { return ELSE;       }
"for"                       { return FOR;        }
"while"                     { return WHILE;      }
"do"                        { return DO;         }
"switch"                    { return SWITCH;     }
"case"                      { return CASE;       }
"default"                   { return DEFAULT;    }
"break"                     { return BREAK;      }
"continue"                  { return CONTINUE;   }
"enum"                      { return ENUM;       }
"typedef"                   { return TYPEDEF;    }
"extern"                    { return EXTERN;     }
"return"                    { return RETURN;     }
"union"                     { return UNION;      }
"goto"                      { return GOTO;       }
([a-zA-Z_])([a-zA-Z_0-9])*  { return ID;         }
[0-9]+("."?[0-9]+)?         { return NUM;        }

%%

int yywrap() {
    printf("exec yywrap()\n");
    return 1;
}