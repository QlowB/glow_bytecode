%{
    #include "bytecode_assembler.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    
    glow_bytecode_block* program;

    char error_type[512];
    
    extern int yylex();

    void yyerror(const char *s) {
        printf("parser error: ");
        if (strlen(s) < sizeof(error_type))
            strcpy(error_type, s);
    }
%}

%union
{
    glow_bytecode_block* program;
    glow_assembler_instruction* instruction;
    char* string;
    int token;
    struct op_t_ {
        int type;
        char* string;
    } operand;
}

%token <string> TIDENTIFIER TINTEGER TDOUBLE THEX_INTEGER
%token <token> TCOMMA TNEWLINE TEOF TCOLON
%type <program> statements
%type <operand> operand
%type <instruction> statement
%type <string> jump_mark

%start program

%%

program:
    pnl statements pnl {
        program = $2;
    }
|
    pnl
    {
        program = malloc(sizeof(glow_bytecode_block));
        glow_init_block(program);
    }
;

statements:
statement {
    $$ = malloc(sizeof(glow_bytecode_block));
    glow_init_block($$);
    {
        int resp = glow_compile_instruction($$, $<instruction>1);
        if (resp)
            return 1;
    }
}
|
statements nl statement {
    $$ = $1;
    {
        int resp = glow_compile_instruction($$, $<instruction>3);
        if (resp)
            return 1;
    }
}
;

statement:
TIDENTIFIER {
    $$ = malloc(sizeof(glow_assembler_instruction));
    glow_init_instruction($$);
    $$->operation = $1;
}
|
TIDENTIFIER operand
{
    $$ = malloc(sizeof(glow_assembler_instruction));
    glow_init_instruction($$);
    $$->operation = $1;
    $$->operand_1 = $2.string;
    $$->operand_1_type = $2.type;
}
|
jump_mark {
    $$ = malloc(sizeof(glow_assembler_instruction));
    glow_init_instruction($$);
    $$->is_jump_mark = 1;
    $$->operation = $1;
}
;


operand:
TIDENTIFIER {
    $$.string = $1;
    $$.type = GLOW_IDENTIFIER;
}
|
TDOUBLE {
    $$.string = $1;
    $$.type = GLOW_FLOAT;
}
|
TINTEGER {
    $$.string = $1;
    $$.type = GLOW_INTEGER;
}
|
THEX_INTEGER {
    $$.string = $1;
    $$.type = GLOW_HEX_INTEGER;
}
;

jump_mark:
TIDENTIFIER TCOLON {
    $$ = $1;
};

nl:
    TNEWLINE
|
    nl TNEWLINE
;

pnl:
    // empty
|
    nl
;

%%

