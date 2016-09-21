#include <stdio.h>
#include <stdlib.h>
#include "bytecode_interpreter.h"
#include "bytecode.h"

#include "glas/glob_headers.h"

char* load_bytecode(const char* path);

int main(int argc, char** argv)
{
    char* glob_sources = 0;

    for (int i = 1; i < argc; i++) {
        glob_sources = argv[i];
    }
    
    if (glob_sources == 0) {
        printf("please specify an input file.\n");
        return 1;
    }

    char* bytecode = load_bytecode(glob_sources);

    /*for (int i = 0; i < 20; i++) {
        printf("%x\n", (int) bytecode[i]);
    }*/

    glow_runtime_env runtime;

    glow_init_runtime(&runtime, bytecode);
    glow_process_instructions(&runtime);
    glow_destroy_runtime(&runtime);

    free(bytecode);
    bytecode = 0;

    return 0;
}


char* load_bytecode(const char* path)
{
    FILE* file = fopen(path, "r");
    fseek(file, 0L, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    if (file_size < sizeof(glow_glob_file))
        exit(1); // error

    glow_glob_file ggh;
    fread(&ggh, sizeof(ggh), 1, file);

    fseek(file, ggh.bytecode_offset, SEEK_SET);

    char* bytecode = malloc(file_size - ggh.bytecode_offset);
    fread(bytecode, file_size - ggh.bytecode_offset, 1, file);

    fclose(file);

    return bytecode;
}


