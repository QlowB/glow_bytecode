#include "bytecode_assembler.h"

#include "glob_headers.h"
#include "../integers.h"
#include "../bytecode.h"

#include "instruction_compiler.h"

#include "conversion_table.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static const int new_buffer_size = 1024;
static const int new_jump_marks_size = 32;
static const int new_jumps_size = 32;

char glow_error_message[1024];


int glow_link(glow_bytecode_block* program)
{
    for (int i = 0; i < program->jumps_used_count; i++) {
        glow_jump* jmp = &program->jumps[i];
        const char* name = jmp->name;
        int found = 0;
        for (int j = 0; j < program->jump_marks_used_count; j++) {
            glow_jump_mark* jmp_mark = &program->jump_marks[j];
            if (strcmp(jmp_mark->name, name) == 0) {
                glow_int32 reljump = (glow_int32) jmp_mark->where -
                        (glow_int32) jmp->from_where;
                char* where = program->buffer + jmp->link_pos;
                *(glow_int32*) where = reljump;
                found = 1;
                break;
            }
        }

        if (found == 0) {
            if (strlen(name) < 900)
                sprintf(glow_error_message, "cannot find jump label %s", name);
            else
                sprintf(glow_error_message, "cannot find jump label");
            return 1;
        }
    }

    return 0;
}


void glow_init_block(glow_bytecode_block* block)
{
    block->buffer_size = new_buffer_size;
    block->buffer = malloc(block->buffer_size);
    block->used_size = 0;

    block->jump_marks_count = new_jump_marks_size;
    block->jump_marks = malloc(block->jump_marks_count * sizeof *block->jump_marks);
    block->jump_marks_used_count = 0;

    block->jumps_count = new_jumps_size;
    block->jumps = malloc(block->jumps_count * sizeof *block->jumps);
    block->jumps_used_count = 0;
}


void glow_destroy_block(glow_bytecode_block* block)
{
    free(block->buffer);
    free(block->jump_marks);
    for (int i = 0; i < block->jumps_used_count; i++) {
        free((void*) block->jumps[i].name);
    }
    free(block->jumps);
    memset(block, 0, sizeof *block);
}


void glow_init_instruction(glow_assembler_instruction* inst)
{
    memset(inst, 0, sizeof(*inst));
}


long long glow_get_integer_operand64(const glow_assembler_instruction* inst, int operand)
{
    long long val = 0;
    char* op = 0;

    switch(operand) {
    case 1:
        op = inst->operand_1;
        break;
    default:
        return 0;
    }

    if (inst->operand_1_type == GLOW_INTEGER)
        val = atoll(op);
    else if (inst->operand_1_type == GLOW_HEX_INTEGER)
        val = strtoll(op + 2, 0, 16);
    else if (inst->operand_1_type == GLOW_FLOAT) {
        double d = atof(op);
        val = *((glow_uint64*) &d);
    }
    return val;
}


void glow_add_bytecode(glow_bytecode_block* block, const char* bytes, int byte_count)
{
    while (block->used_size + byte_count >= block->buffer_size) {
        char* new_buffer = malloc(block->buffer_size * 2);
        memcpy(new_buffer, block->buffer, block->buffer_size);
        memset(new_buffer + block->buffer_size, 0, block->buffer_size);
        block->buffer_size *= 2;
        free(block->buffer);
        block->buffer = new_buffer;
    }
    
    memcpy(block->buffer + block->used_size, bytes, byte_count);
    block->used_size += byte_count;
}


void glow_add_jump_mark(glow_bytecode_block* block, const char* name, long where)
{
    if (block->jump_marks_used_count + 1 >= block->jump_marks_count) {
        glow_jump_mark* new_buffer = malloc(block->jump_marks_count * 2);
        memcpy(new_buffer, block->jump_marks,
               block->jump_marks_count * sizeof(glow_jump_mark));
        memset(new_buffer + block->jump_marks_count,
               0, block->jump_marks_count * sizeof(glow_jump_mark));
        block->jump_marks_count *= 2;
        free(block->jump_marks);
        block->jump_marks = new_buffer;
    }

    block->jump_marks[block->jump_marks_used_count].name = name;
    block->jump_marks[block->jump_marks_used_count].where = where;
    block->jump_marks_used_count++;
}


void glow_add_jump(glow_bytecode_block* block,
                   const char* name, long link_pos, long from_where)
{
    if (block->jumps_used_count + 1 >= block->jumps_count) {
        glow_jump* new_buffer = malloc(block->jumps_count * 2);
        memcpy(new_buffer, block->jumps,
               block->jumps_count * sizeof(glow_jump));
        memset(new_buffer + block->jumps_count,
               0, block->jump_marks_count * sizeof(glow_jump));
        block->jumps_count *= 2;
        free(block->jumps);
        block->jumps = new_buffer;
    }

    block->jumps[block->jumps_used_count].name = name;
    block->jumps[block->jumps_used_count].link_pos = link_pos;
    block->jumps[block->jumps_used_count].from_where = from_where;
    block->jumps_used_count++;
}


/*!
 * \brief compiles one instruction and adds it to the block
 *
 * \param inst the assembly data to compile
 * \return 0 if succeeded, nonzero otherwise
 */
int glow_compile_instruction(glow_bytecode_block* block, glow_assembler_instruction* inst)
{



    if (inst->is_jump_mark) {
        glow_add_jump_mark(block, inst->operation, block->used_size);
        return 0;
    }
    else if (inst->operation == 0) {
        strcpy(glow_error_message, GLOW_FATAL_ERROR_MESSAGE);
        return 1;
    }

    char code_buf[GLOW_MAX_BYTES_PER_INSTRUCTION];

    glow_instruction operation = glow_lookup_instruction(inst->operation);

    int bytes_written = 0;

    static const int ONE_BYTE = 1;
    static const int TWO_BYTES = 2;
    static const int FOUR_BYTES = 4;
    static const int EIGHT_BYTES = 8;

    switch (operation) {

    /*
     * one byte argument
     */
    case GLOW_LOAD_CONSTANT_INT8:

        bytes_written = glow_compile_single_int(
                code_buf, operation, inst, ONE_BYTE);
        break;

    case GLOW_LOAD_CONSTANT_INT16:

        bytes_written = glow_compile_single_int(
                code_buf, operation, inst, TWO_BYTES);
        break;

    case GLOW_LOAD_CONSTANT_INT32:
    case GLOW_LOAD_LOCAL_INT8:
    case GLOW_LOAD_LOCAL_INT16:
    case GLOW_LOAD_LOCAL_INT32:
    case GLOW_LOAD_LOCAL_INT64:
    case GLOW_LOAD_LOCAL_REFERENCE:
    case GLOW_STORE_LOCAL_INT8:
    case GLOW_STORE_LOCAL_INT16:
    case GLOW_STORE_LOCAL_INT32:
    case GLOW_STORE_LOCAL_INT64:
    case GLOW_STORE_LOCAL_REFERENCE:

    case GLOW_ALLOCATE_OBJECT:
    case GLOW_DELETE_OBJECT:
    
    case GLOW_LOAD_FROM_FIELD_8:
    case GLOW_LOAD_FROM_FIELD_16:
    case GLOW_LOAD_FROM_FIELD_32:
    case GLOW_LOAD_FROM_FIELD_64:
    case GLOW_LOAD_FROM_FIELD_REFERENCE:
    
    case GLOW_STORE_IN_FIELD_8:
    case GLOW_STORE_IN_FIELD_16:
    case GLOW_STORE_IN_FIELD_32:
    case GLOW_STORE_IN_FIELD_64:
    case GLOW_STORE_IN_FIELD_REFERENCE:
    
    case GLOW_CALL_NATIVE:

        bytes_written = glow_compile_single_int(
                code_buf, operation, inst, FOUR_BYTES);
        break;

    case GLOW_LOAD_CONSTANT_INT64:

        bytes_written = glow_compile_single_int(
                code_buf, operation, inst, EIGHT_BYTES);
        break;

    case GLOW_JUMP:
    case GLOW_JUMP_IF_ZERO:
    // case GLOW_JUMP_IF_EQUAL:
    case GLOW_JUMP_IF_NOT_ZERO:
    // case GLOW_JUMP_IF_NOT_EQUAL:
    case GLOW_JUMP_IF_GREATER:
    case GLOW_JUMP_IF_LESS:
    case GLOW_JUMP_IF_GREATER_OR_EQUAL:
    case GLOW_JUMP_IF_LESS_OR_EQUAL:

        bytes_written = glow_compile_jump(
                code_buf, block, operation, inst);
        break;

    case GLOW_NO_OPERATION:
    case GLOW_POP_8:
    case GLOW_POP_16:
    case GLOW_POP_32:
    case GLOW_POP_64:
    case GLOW_COMPARE_INT8:
    case GLOW_COMPARE_INT16:
    case GLOW_COMPARE_INT32:
    case GLOW_COMPARE_INT64:
    case GLOW_COMPARE_UINT8:
    case GLOW_COMPARE_UINT16:
    case GLOW_COMPARE_UINT32:
    case GLOW_COMPARE_UINT64:
    case GLOW_COMPARE_FLOAT32:
    case GLOW_COMPARE_FLOAT64:
    case GLOW_SIGN_EXTEND_INT8_TO_INT16:
    case GLOW_SIGN_EXTEND_INT8_TO_INT32:
    case GLOW_SIGN_EXTEND_INT8_TO_INT64:
    case GLOW_SIGN_EXTEND_INT16_TO_INT32:
    case GLOW_SIGN_EXTEND_INT16_TO_INT64:
    case GLOW_SIGN_EXTEND_INT32_TO_INT64:
    case GLOW_NEGATE_INT8:
    case GLOW_NEGATE_INT16:
    case GLOW_NEGATE_INT32:
    case GLOW_NEGATE_INT64:
    case GLOW_NEGATE_FLOAT32:
    case GLOW_NEGATE_FLOAT64:
    case GLOW_DUPLICATE_8:
    case GLOW_DUPLICATE_16:
    case GLOW_DUPLICATE_32:
    case GLOW_DUPLICATE_64:
    case GLOW_ADD_INT8:
    case GLOW_ADD_INT16:
    case GLOW_ADD_INT32:
    case GLOW_ADD_INT64:
    case GLOW_ADD_FLOAT32:
    case GLOW_ADD_FLOAT64:
    case GLOW_MULT_INT8:
    case GLOW_MULT_INT16:
    case GLOW_MULT_INT32:
    case GLOW_MULT_INT64:
    case GLOW_MULT_UINT8:
    case GLOW_MULT_UINT16:
    case GLOW_MULT_UINT32:
    case GLOW_MULT_UINT64:
    case GLOW_MULT_FLOAT32:
    case GLOW_MULT_FLOAT64:
    case GLOW_DIV_INT8:
    case GLOW_DIV_INT16:
    case GLOW_DIV_INT32:
    case GLOW_DIV_INT64:
    case GLOW_DIV_UINT8:
    case GLOW_DIV_UINT16:
    case GLOW_DIV_UINT32:
    case GLOW_DIV_UINT64:
    case GLOW_DIV_FLOAT32:
    case GLOW_DIV_FLOAT64:
    case GLOW_AND_INT8:
    case GLOW_AND_INT16:
    case GLOW_AND_INT32:
    case GLOW_AND_INT64:
    case GLOW_OR_INT8:
    case GLOW_OR_INT16:
    case GLOW_OR_INT32:
    case GLOW_OR_INT64:
    case GLOW_XOR_INT8:
    case GLOW_XOR_INT16:
    case GLOW_XOR_INT32:
    case GLOW_XOR_INT64:
    case GLOW_NOT_INT8:
    case GLOW_NOT_INT16:
    case GLOW_NOT_INT32:
    case GLOW_NOT_INT64:
    case GLOW_INT8_TO_FLOAT32:
    case GLOW_INT16_TO_FLOAT32:
    case GLOW_INT32_TO_FLOAT32:
    case GLOW_INT64_TO_FLOAT32:
    case GLOW_INT8_TO_FLOAT64:
    case GLOW_INT16_TO_FLOAT64:
    case GLOW_INT32_TO_FLOAT64:
    case GLOW_INT64_TO_FLOAT64:
    case GLOW_FLOAT32_TO_INT8:
    case GLOW_FLOAT32_TO_INT16:
    case GLOW_FLOAT32_TO_INT32:
    case GLOW_FLOAT32_TO_INT64:
    case GLOW_FLOAT64_TO_INT8:
    case GLOW_FLOAT64_TO_INT16:
    case GLOW_FLOAT64_TO_INT32:
    case GLOW_FLOAT64_TO_INT64:
    case GLOW_UINT8_TO_UINT64:
    case GLOW_UINT16_TO_UINT64:
    case GLOW_UINT32_TO_UINT64:
    case GLOW_UINT8_TO_UINT32:
    case GLOW_UINT16_TO_UINT32:
    case GLOW_UINT64_TO_UINT32:
    case GLOW_UINT8_TO_UINT16:
    case GLOW_UINT32_TO_UINT16:
    case GLOW_UINT64_TO_UINT16:
    case GLOW_UINT16_TO_UINT8:
    case GLOW_UINT32_TO_UINT8:
    case GLOW_UINT64_TO_UINT8:
    case GLOW_FLOAT32_TO_FLOAT64:
    case GLOW_FLOAT64_TO_FLOAT32:
    case GLOW_MOD_INT8:
    case GLOW_MOD_INT16:
    case GLOW_MOD_INT32:
    case GLOW_MOD_INT64:
    case GLOW_MOD_UINT8:
    case GLOW_MOD_UINT16:
    case GLOW_MOD_UINT32:
    case GLOW_MOD_UINT64:
    case GLOW_SUB_INT8:
    case GLOW_SUB_INT16:
    case GLOW_SUB_INT32:
    case GLOW_SUB_INT64:
    case GLOW_SUB_FLOAT32:
    case GLOW_SUB_FLOAT64:
    
    case GLOW_LOAD_FROM_ARRAY_8:
    case GLOW_LOAD_FROM_ARRAY_16:
    case GLOW_LOAD_FROM_ARRAY_32:
    case GLOW_LOAD_FROM_ARRAY_64:
    case GLOW_LOAD_FROM_ARRAY_REFERENCE:
    case GLOW_STORE_IN_ARRAY_8:
    case GLOW_STORE_IN_ARRAY_16:
    case GLOW_STORE_IN_ARRAY_32:
    case GLOW_STORE_IN_ARRAY_64:
    case GLOW_STORE_IN_ARRAY_REFERENCE:
    
    case GLOW_EXIT:

        bytes_written = glow_compile_single(code_buf, operation);
        break;


    default:

        if (strlen(inst->operation) < 700)
            sprintf(glow_error_message, "unknown operation: %s", inst->operation);
        else
            sprintf(glow_error_message, "unknown operation");
        bytes_written = -1;
        break;
    }

    if (bytes_written <= 0)
        return 1;

    // if no errors detected, write compiled bytecode to buffer and return
    glow_add_bytecode(block, code_buf, bytes_written);
    return 0;
}


void glow_save_glob(glow_bytecode_block* block, FILE* stream)
{
    glow_glob_header header;
    memset(&header, 0, sizeof header);
    header.glob[0] = 'G';
    header.glob[1] = 'L';
    header.glob[2] = 'O';
    header.glob[3] = 'B';
    
    header.version[0] = 1;
    
    header.bytecode_offset = sizeof header;
    
    fwrite(&header, sizeof header, 1, stream);
    fwrite(block->buffer, block->used_size, 1, stream);
}


void glow_load_glob(glow_bytecode_block* block, FILE* file)
{
    fseek(file, 0L, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0L, SEEK_SET);
    
    if (file_size < sizeof(glow_glob_header))
        exit(1); // error
    
    glow_glob_header ggh;
    fread(&ggh, sizeof(ggh), 1, file);
    
    fseek(file, ggh.bytecode_offset, SEEK_SET);
    
    char* bytecode = malloc(file_size - ggh.bytecode_offset);
    fread(bytecode, file_size - ggh.bytecode_offset, 1, file);
    
    
    block->buffer = bytecode;
    block->buffer_size = file_size - ggh.bytecode_offset;
    block->used_size = block->buffer_size;
}


void glow_get_last_error(char* err_msg, int buffer_size)
{
    int errlen = strlen(glow_error_message);
    if (errlen >= buffer_size) {
        memcpy(err_msg, glow_error_message, buffer_size - 1);
        err_msg[buffer_size - 1] = 0;
    } else {
        memcpy(err_msg, glow_error_message, errlen + 1);
    }
}


