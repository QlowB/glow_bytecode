#include "bytecode_assembler.h"
#include "conversion_table.h"
#include "run_options.h"
#include "../glow_error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



extern glow_bytecode_block* program;
extern const char* glow_error_message;

// bison error message
extern char error_type[512];

extern int yyparse(void);
extern FILE* yyin;

int main(int argc, const char** argv)
{
    glow_run_options options;
    glow_read_options(&options, argc, argv);

    if (glow_has_option(&options, "-h")) {
        glow_print_reference(stdout);
        exit(0);
    }

    // scan arguments
    if (options.input_file != 0) {
        yyin = fopen(options.input_file, "r");
    }

    int response = yyparse();

    if (options.input_file != 0) {
        fclose(yyin);
    }


    if (program != 0 && response == 0) {

        int link_status = glow_link(program);

        if (link_status != 0) {
            printf("linking error: %s\n", glow_get_last_error());
            return 1;
        }

        FILE* out_file;
        if (options.output_file != 0) {
            out_file = fopen(options.output_file, "w");
        } else {
            out_file = stdout;
        }
        glow_save_glob(&program->object_content, out_file);
        if (options.output_file != 0) {
            fclose(out_file);
        }
    } else {
        if (strcmp(error_type, "syntax error") == 0) {
            printf("syntax error detected.\n");
        } else {
            printf("%s\n", glow_get_last_error());
        }
        printf("compilation aborted because of errors.\n");
        return 1;
    }
    
    return 0;
}

