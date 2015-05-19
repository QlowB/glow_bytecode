%{
    #include "bytecode_assembler.h"
    #include "parser.h"

    #include <string.h>
    
    #define SAVE_TOKEN yylval.string = malloc(yyleng + 1); strcpy(yylval.string, yytext);
    #define TOKEN(t) (yylval.token = t)
    int yywrap() { return 1; }
%}

%%

[ \t]                   ;
\/\*[.\n]+\*\/          ; // multiline comments
\/\/.+\n                return TNEWLINE; // comments
[#;].+\n                return TNEWLINE; // other forms of comments
[\n]                    return TNEWLINE;
[a-zA-Z_][a-zA-Z0-9_]*  SAVE_TOKEN; return TIDENTIFIER;
\-?[0-9]+\.[0-9]*       SAVE_TOKEN; return TDOUBLE;
\-?[0-9]+               SAVE_TOKEN; return TINTEGER;
0x\-?[0-9A-Fa-f]+       SAVE_TOKEN; return THEX_INTEGER;
","                     return TCOMMA;
":"                     return TCOLON;
.                       yyterminate();
%%

