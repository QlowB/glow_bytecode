#include "instruction_compiler.h"
#include "bytecode_assembler.h"
#include "../bytecode.h"
#include "../integers.h"
#include "../glow_error.h"

#include <string.h>
#include <stdlib.h>
#include <limits.h>


int glow_compile_single_int(char* buf, glow_instruction oper,
                            const struct glow_asm_inst_* inst,
                            int argument_bytes)
{
    buf[0] = oper;
    if (inst->operand_1 != 0) {
        if (inst->operand_1_type != GLOW_INTEGER &&
                inst->operand_1_type != GLOW_HEX_INTEGER) {
            char buffer[1024];
            sprintf(buffer, "operand of %s must be an integer", inst->operation);
            glow_set_last_error(buffer);
            return -1;
        }
        long long val = glow_get_integer_operand64(inst, 1);

        long long max;
        long long min;

        switch(argument_bytes) {
        case 8:
            max = LLONG_MAX;
            min = LLONG_MIN;
            break;
        case 4:
            max = 0xFFFFFFFFLL;
            min = -0x80000000LL;
            break;
        case 2:
            max = 0xFFFFLL;
            min = -0x8000LL;
            break;
        case 1:
            max = 0xFFLL;
            min = -0x80LL;
            break;
        }

        if (val > max || val < min) {
            char buffer[1024];
            sprintf(buffer, "operand of %s is out of range", inst->operation);
            glow_set_last_error(buffer);
            return -1;
        }

        // little endian output
        switch(argument_bytes) {
        case 8:
            *((glow_uint64*) &buf[1]) = val;
            break;
        case 4:
            *((glow_uint32*) &buf[1]) = val;
            break;
        case 2:
            *((glow_uint16*) &buf[1]) = val;
            break;
        case 1:
            *((glow_uint8*) &buf[1]) = val;
            break;
        }
        return argument_bytes + 1;
    }
    return -1;
}


int glow_compile_single(char* buf, glow_instruction oper)
{
    buf[0] = oper;
    return 1;
}


int glow_compile_jump(char* code_buf, glow_bytecode_block* block, glow_instruction oper,
                      const struct glow_asm_inst_* inst, glow_jump_type type)
{
    code_buf[0] = oper;
    memset(code_buf + 1, 0, 4);

    if (inst->operand_1 == 0) {
        char buffer[1024];
        sprintf(buffer,
                "%s needs a destination operand (integer or jump label)",
                inst->operation);
        glow_set_last_error(buffer);
        return -1;
    }

    long length = strlen(inst->operand_1);
    char* copy = malloc(length + 1);
    memcpy(copy, inst->operand_1, length + 1);
    glow_add_jump(block, copy, block->used_size + 1, block->used_size + 5, type);
    return 5;
}


