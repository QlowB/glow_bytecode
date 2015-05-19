#include "bytecode_interpreter.h"
#include "bytecode.h"

#include "integers.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#if __GNUC__
#if __x86_64__ || __ppc64__
#define ARCHITECTURE_X86_64
#else
#define ARCHITECTURE_X86
#endif
#elif _WIN32 || _WIN64
#if _WIN64
#define ARCHITECTURE_X86_64
#else
#define ARCHITECTURE_X86
#endif
#endif


static void push_operand8(glow_runtime_env*, glow_uint8);
static void push_operand16(glow_runtime_env*, glow_uint16);
static void push_operand32(glow_runtime_env*, glow_uint32);
static void push_operand64(glow_runtime_env*, glow_uint64);
static void push_operand_word(glow_runtime_env*, glow_word);
static void push_operand_f32(glow_runtime_env*, glow_float32);
static void push_operand_f64(glow_runtime_env*, glow_float64);

static glow_uint8 pop_operand8(glow_runtime_env*);
static glow_uint16 pop_operand16(glow_runtime_env*);
static glow_uint32 pop_operand32(glow_runtime_env*);
static glow_uint64 pop_operand64(glow_runtime_env*);
static glow_word pop_operand_word(glow_runtime_env*);
static glow_float32 pop_operand_f32(glow_runtime_env*);
static glow_float64 pop_operand_f64(glow_runtime_env*);

static glow_int64 print_int64(glow_runtime_env* runtime)
{
    glow_int64 value = *((glow_int64*) (runtime->operand_stack_top - 8));
    printf("%ld\n", value);
    return 0;
}


void glow_init_runtime(glow_runtime_env* runtime, void* instruction_memory)
{
    runtime->instruction_memory = instruction_memory;
    runtime->instruction_pointer = instruction_memory;

    runtime->call_stack_base = malloc(glow_call_stack_size);
    runtime->operand_stack_base = malloc(glow_operand_stack_size);

    runtime->call_stack_top = runtime->call_stack_base;
    runtime->operand_stack_top = runtime->operand_stack_base;

    runtime->native_function_count = 1;
    runtime->native_functions = malloc(runtime->native_function_count * sizeof(glow_native_func));
    runtime->native_functions[0] = &print_int64;
}


void glow_destroy_runtime(glow_runtime_env* runtime)
{
    free(runtime->call_stack_base);
    free(runtime->operand_stack_base);
    free(runtime->native_functions);
    memset(runtime, 0, sizeof *runtime);
}


void glow_process_instructions(glow_runtime_env* runtime)
{
    
    glow_uint64 temp64;
    glow_uint64 temp64_2;
    
    glow_int64 temps64;
    glow_int64 temps64_2;
    
    glow_int8 temps8;
    glow_int8 temps8_2;
    glow_uint8 temp8;
    glow_uint8 temp8_2;
    glow_int16 temps16;
    glow_int16 temps16_2;
    glow_uint16 temp16;
    glow_uint16 temp16_2;
    glow_int32 temps32;
    glow_int32 temps32_2;
    glow_uint32 temp32;
    glow_uint32 temp32_2;
    // glow_word temp_word;
    
    glow_uint16 val;
    
start:
    //printf("interpreting\n");
    val = *runtime->instruction_pointer;
    if ((val & 0x00F0) == 0x00F0) {
        val = (val << 8) | *((unsigned char*) runtime->instruction_pointer + 1);
    }

    //printf("val = %x\n", (int) val);
    switch(val) {
            
    case GLOW_NO_OPERATION:
        runtime->instruction_pointer += 1;
        break;

    /*
     * load constants
     */

    case GLOW_LOAD_CONSTANT_INT8:
        push_operand8(runtime, *(glow_uint8*)(runtime->instruction_pointer + 1));
        runtime->instruction_pointer += 2;
        break;
    case GLOW_LOAD_CONSTANT_INT16:
        push_operand16(runtime, *(glow_uint16*)(runtime->instruction_pointer + 1));
        runtime->instruction_pointer += 3;
        break;
    case GLOW_LOAD_CONSTANT_INT32:
        push_operand32(runtime, *(glow_uint32*)(runtime->instruction_pointer + 1));
        runtime->instruction_pointer += 5;
        break;
    case GLOW_LOAD_CONSTANT_INT64:
        push_operand64(runtime, *((glow_uint64*) (runtime->instruction_pointer + 1)));
        runtime->instruction_pointer += 9;
        break;


    /*
     * load and store local
     */
    case GLOW_LOAD_LOCAL_INT8:
        temps32 = *(glow_int32*)(runtime->instruction_pointer + 1); // temp = stack offset
        push_operand8(runtime, *((glow_uint8*) (runtime->call_stack_top + temps32)));
        runtime->instruction_pointer += 5;
        break;
    case GLOW_LOAD_LOCAL_INT16:
        temps32 = *(glow_int32*)(runtime->instruction_pointer + 1); // temp = stack offset
        push_operand16(runtime, *((glow_uint16*) (runtime->call_stack_top + temps32)));
        runtime->instruction_pointer += 5;
        break;
    case GLOW_LOAD_LOCAL_INT32:
        temps32 = *(glow_int32*)(runtime->instruction_pointer + 1); // temp = stack offset
        push_operand32(runtime, *((glow_uint32*) (runtime->call_stack_top + temps32)));
        runtime->instruction_pointer += 5;
        break;
    case GLOW_LOAD_LOCAL_INT64:
        temps32 = *(glow_int32*)(runtime->instruction_pointer + 1); // temp = stack offset
        push_operand64(runtime, *((glow_uint64*) (runtime->call_stack_top + temps32)));
        runtime->instruction_pointer += 5;
        break;

    case GLOW_LOAD_LOCAL_REFERENCE:
        temps32 = *(glow_int32*)(runtime->instruction_pointer + 1); // temp = stack offset
        push_operand_word(runtime, *((glow_word*) (runtime->call_stack_top + temps32)));
        runtime->instruction_pointer += 5;
        break;

    case GLOW_STORE_LOCAL_INT8: {
        glow_int32 temp = *(glow_int32*)(runtime->instruction_pointer + 1); // temp = stack offset
        glow_uint8 temp2 = pop_operand8(runtime);
        *((glow_uint8*) (((char*) runtime->call_stack_top) + temp)) = temp2;
        runtime->instruction_pointer += 5;
        break;
    }
    case GLOW_STORE_LOCAL_INT16: {
        glow_int32 temp = *(glow_int32*)(runtime->instruction_pointer + 1); // temp = stack offset
        glow_uint16 temp2 = pop_operand16(runtime);
        *((glow_uint16*) (((char*) runtime->call_stack_top) + temp)) = temp2;
        runtime->instruction_pointer += 5;
        break;
    }
    case GLOW_STORE_LOCAL_INT32: {
        glow_int32 temp = *(glow_int32*)(runtime->instruction_pointer + 1); // temp = stack offset
        glow_uint32 temp2 = pop_operand32(runtime);
        *((glow_uint32*) (((char*) runtime->call_stack_top) + temp)) = temp2;
        runtime->instruction_pointer += 5;
        break;
    }
    case GLOW_STORE_LOCAL_INT64: {
        glow_int32 temp = *(glow_int32*)(runtime->instruction_pointer + 1); // temp = stack offset
        glow_uint64 temp2 = pop_operand64(runtime);
        *((glow_uint64*) (((char*) runtime->call_stack_top) + temp)) = temp2;
        runtime->instruction_pointer += 5;
        break;
    }
    case GLOW_STORE_LOCAL_REFERENCE: {
        glow_int32 temp = *(glow_int32*)(runtime->instruction_pointer + 1); // temp = stack offset
        glow_word temp2 = pop_operand_word(runtime);
        *((glow_word*) (((char*) runtime->call_stack_top) + temp)) = temp2;
        runtime->instruction_pointer += 5;
        break;
    }

    case GLOW_POP_8:
        runtime->operand_stack_top -= 1;
        runtime->instruction_pointer += 1;
        break;
    case GLOW_POP_16:
        runtime->operand_stack_top -= 2;
        runtime->instruction_pointer += 1;
        break;
    case GLOW_POP_32:
        runtime->operand_stack_top -= 4;
        runtime->instruction_pointer += 1;
        break;
    case GLOW_POP_64:
        runtime->operand_stack_top -= 8;
        runtime->instruction_pointer += 1;
        break;

    case GLOW_COMPARE_INT8:
        temps8 = pop_operand8(runtime);
        temps8_2 = pop_operand8(runtime);
        if (temps8 < temps8_2)
            push_operand8(runtime, 1);
        else if (temps8 == temps8_2)
            push_operand8(runtime, 0);
        else
            push_operand8(runtime, 255); // is -1 when viewed as a signed 8-bit int
        runtime->instruction_pointer += 1;
        break;
        
    case GLOW_COMPARE_INT16:
        temps16 = pop_operand16(runtime);
        temps16_2 = pop_operand16(runtime);
        if (temps16 < temps16_2)
            push_operand8(runtime, 1);
        else if (temps16 == temps16_2)
            push_operand8(runtime, 0);
        else
            push_operand8(runtime, 255); // is -1 when viewed as a signed 8-bit int
        runtime->instruction_pointer += 1;
        break;
        
    case GLOW_COMPARE_INT32:
        temps32 = pop_operand32(runtime);
        temps32_2 = pop_operand32(runtime);
        if (temps32 < temps32_2)
            push_operand8(runtime, 1);
        else if (temps32 == temps32_2)
            push_operand8(runtime, 0);
        else
            push_operand8(runtime, 255); // is -1 when viewed as a signed 8-bit int
        runtime->instruction_pointer += 1;
        break;
        
    case GLOW_COMPARE_INT64:
        temps64 = pop_operand64(runtime);
        temps64_2 = pop_operand64(runtime);
        if (temps64 < temps64_2)
            push_operand8(runtime, 1);
        else if (temps64 == temps64_2)
            push_operand8(runtime, 0);
        else
            push_operand8(runtime, 255); // is -1 when viewed as a signed 8-bit int
        runtime->instruction_pointer += 1;
        break;
        
    case GLOW_COMPARE_UINT8:
        temp8 = pop_operand8(runtime);
        temp8_2 = pop_operand8(runtime);
        if (temp8 < temp8_2)
            push_operand8(runtime, 1);
        else if (temp8 == temp8_2)
            push_operand8(runtime, 0);
        else
            push_operand8(runtime, 255); // is -1 when viewed as a signed 8-bit int
        runtime->instruction_pointer += 1;
        break;
        
    case GLOW_COMPARE_UINT16:
        temp16 = pop_operand16(runtime);
        temp16_2 = pop_operand16(runtime);
        if (temp16 < temp16_2)
            push_operand8(runtime, 1);
        else if (temp16 == temp16_2)
            push_operand8(runtime, 0);
        else
            push_operand8(runtime, 255); // is -1 when viewed as a signed 8-bit int
        runtime->instruction_pointer += 1;
        break;
        
    case GLOW_COMPARE_UINT32:
        temp32 = pop_operand32(runtime);
        temp32_2 = pop_operand32(runtime);
        if (temp32 < temp32_2)
            push_operand8(runtime, 1);
        else if (temp32 == temp32_2)
            push_operand8(runtime, 0);
        else
            push_operand8(runtime, 255); // is -1 when viewed as a signed 8-bit int
        runtime->instruction_pointer += 1;
        break;
        
    case GLOW_COMPARE_UINT64:
        temp64 = pop_operand64(runtime);
        temp64_2 = pop_operand64(runtime);
        if (temp64 < temp64_2)
            push_operand8(runtime, 1);
        else if (temp64 == temp64_2)
            push_operand8(runtime, 0);
        else
            push_operand8(runtime, 255); // is -1 when viewed as a signed 8-bit int
        runtime->instruction_pointer += 1;
        break;
        

#define CONDITIONAL_JUMP(id, condition) \
    case id: { \
        glow_int8 temp = (glow_int8) pop_operand8(runtime); \
        glow_int32 jmp_offset = *((glow_int32*) (runtime->instruction_pointer + 1)); \
        if (temp condition) { \
            runtime->instruction_pointer += jmp_offset; \
        } \
        runtime->instruction_pointer += 5; \
        break; \
    }
        CONDITIONAL_JUMP(GLOW_JUMP_IF_ZERO, == 0)
        CONDITIONAL_JUMP(GLOW_JUMP_IF_NOT_ZERO, != 0)
        CONDITIONAL_JUMP(GLOW_JUMP_IF_GREATER, > 0)
        CONDITIONAL_JUMP(GLOW_JUMP_IF_LESS, < 0)
        CONDITIONAL_JUMP(GLOW_JUMP_IF_GREATER_OR_EQUAL, >= 0)
        CONDITIONAL_JUMP(GLOW_JUMP_IF_LESS_OR_EQUAL, <= 0)
        CONDITIONAL_JUMP(GLOW_JUMP, || 1)

    case GLOW_SIGN_EXTEND_INT8_TO_INT16:
        temps8 = pop_operand8(runtime);
        temps16 = temps8;
        push_operand16(runtime, temps16);
        runtime->instruction_pointer += 1;
        break;

    case GLOW_SIGN_EXTEND_INT8_TO_INT32:
        temps8 = pop_operand8(runtime);
        temps32 = temps8;
        push_operand32(runtime, temps32);
        runtime->instruction_pointer += 1;
        break;

    case GLOW_SIGN_EXTEND_INT8_TO_INT64:
        temps8 = pop_operand8(runtime);
        temps64 = temps8;
        push_operand64(runtime, temps64);
        runtime->instruction_pointer += 1;
        break;


    case GLOW_SIGN_EXTEND_INT16_TO_INT32:
        temps16 = pop_operand16(runtime);
        temps32 = temps16;
        push_operand32(runtime, temps32);
        runtime->instruction_pointer += 1;
        break;

    case GLOW_SIGN_EXTEND_INT16_TO_INT64:
        temps16 = pop_operand16(runtime);
        temps64 = temps16;
        push_operand64(runtime, temps64);
        runtime->instruction_pointer += 1;
        break;

    case GLOW_SIGN_EXTEND_INT32_TO_INT64:
        temps32 = pop_operand32(runtime);
        temps64 = temps32;
        push_operand64(runtime, temps64);
        runtime->instruction_pointer += 1;
        break;


    case GLOW_NEGATE_INT8:
        push_operand8(runtime, -pop_operand8(runtime));
        break;

    case GLOW_NEGATE_INT16:
        push_operand16(runtime, -pop_operand16(runtime));
        break;

    case GLOW_NEGATE_INT32:
        push_operand32(runtime, -pop_operand32(runtime));
        break;

    case GLOW_NEGATE_INT64:
        push_operand64(runtime, -pop_operand64(runtime));
        break;

    case GLOW_NEGATE_FLOAT32:
        push_operand_f32(runtime, -pop_operand_f32(runtime));
        break;

    case GLOW_NEGATE_FLOAT64:
        push_operand_f64(runtime, -pop_operand_f64(runtime));
        break;


    case GLOW_DUPLICATE_8: {
        glow_uint8 tmp = pop_operand8(runtime);
        push_operand8(runtime, tmp);
        push_operand8(runtime, tmp);
        break;
    }
    case GLOW_DUPLICATE_16: {
        glow_uint16 tmp = pop_operand16(runtime);
        push_operand16(runtime, tmp);
        push_operand16(runtime, tmp);
        break;
    }
    case GLOW_DUPLICATE_32: {
        glow_uint32 tmp = pop_operand32(runtime);
        push_operand32(runtime, tmp);
        push_operand32(runtime, tmp);
        break;
    }
    case GLOW_DUPLICATE_64: {
        glow_uint64 tmp = pop_operand64(runtime);
        push_operand64(runtime, tmp);
        push_operand64(runtime, tmp);
        break;
    }


    /*
     * arithmetic stuff
     */

    case GLOW_ADD_INT8:
        temp32 = pop_operand8(runtime);
        temp32_2 = pop_operand8(runtime);
        push_operand8(runtime, (glow_int8) temp32 + (glow_int8) temp32_2);
        runtime->instruction_pointer += 1;
        break;

    case GLOW_ADD_INT16:
        temp32 = pop_operand16(runtime);
        temp32_2 = pop_operand16(runtime);
        push_operand16(runtime, (glow_int16) temp32 + (glow_int16) temp32_2);
        runtime->instruction_pointer += 1;
        break;

    case GLOW_ADD_INT32:
        temp32 = pop_operand32(runtime);
        temp32_2 = pop_operand32(runtime);
        push_operand32(runtime, temp32 + temp32_2);
        runtime->instruction_pointer += 1;
        break;

    case GLOW_ADD_INT64:
        temp64 = pop_operand64(runtime);
        temp64_2 = pop_operand64(runtime);
        push_operand64(runtime, temp64 + temp64_2);
        runtime->instruction_pointer += 1;
        break;

    case GLOW_ADD_FLOAT32:
        temp32 = pop_operand32(runtime);
        temp32_2 = pop_operand32(runtime);
        push_operand32(runtime, *(glow_uint32*)
                       ((glow_float32*) &temp32) + *((glow_float32*) &temp32_2));
        runtime->instruction_pointer += 1;
        break;

    case GLOW_ADD_FLOAT64:
        temp64 = pop_operand64(runtime);
        temp64_2 = pop_operand64(runtime);
        push_operand64(runtime, *(glow_uint64*)
                       ((glow_float64*) &temp64) + *((glow_float64*) &temp64_2));
        runtime->instruction_pointer += 1;
        break;



    case GLOW_MULT_INT8:
        temps16 = (glow_int16) (glow_int8) pop_operand8(runtime);
        temps16_2 = (glow_int16) (glow_int8) pop_operand8(runtime);
        temps32 = temps16 * temps16_2;
        push_operand8(runtime, temps16 & 0xFF);
        push_operand8(runtime, (temps16_2 >> 8) & 0xFF);
        runtime->instruction_pointer += 1;
        break;

    case GLOW_MULT_INT16:
        temps32 = (glow_int32) (glow_int16) pop_operand16(runtime);
        temps32_2 = (glow_int32) (glow_int16) pop_operand16(runtime);
        temps64 = temps32 * temps32_2;
        push_operand16(runtime, temps64 & 0xFFFF);
        push_operand16(runtime, (temps64 >> 16) & 0xFFFF);
        runtime->instruction_pointer += 1;
        break;

    case GLOW_MULT_INT32:
        temps64 = (glow_int64) (glow_int32) pop_operand32(runtime);
        temps64_2 = (glow_int64) (glow_int32) pop_operand32(runtime);
        temps64 = temps64 * temps64_2;
        push_operand32(runtime, temps64 & 0xFFFFFFFF);
        push_operand32(runtime, (temps64 >> 16) & 0xFFFFFFFF);
        runtime->instruction_pointer += 1;
        break;

    case GLOW_MULT_INT64:
        temps64 = (glow_int64) pop_operand64(runtime);
        temps64_2 = (glow_int64) pop_operand64(runtime);
#ifdef ARCHITECTURE_X86_64
        asm("imul %%rbx" : "=a"(temps64), "=d"(temps64_2) : "a" (temps64), "b" (temps64_2));
#else
#error "instruction GLOW_MULT_INT64 not yet implemented for x86 architecture."
#endif
        push_operand64(runtime, temps64);
        push_operand64(runtime, temps64_2);
        runtime->instruction_pointer += 1;
        break;

    case GLOW_MULT_UINT8:
        temp16 = pop_operand8(runtime);
        temp16_2 = pop_operand8(runtime);
        temp16 = temp16 * temp16_2;
        push_operand8(runtime, temp16 & 0xFF);
        push_operand8(runtime, (temp16 >> 8) & 0xFF);
        runtime->instruction_pointer += 1;
        break;

    case GLOW_MULT_UINT16:
        temp32 = pop_operand16(runtime);
        temp32_2 = pop_operand16(runtime);
        temp32 = temp32 * temp32_2;
        push_operand16(runtime, temp32 & 0xFFFF);
        push_operand16(runtime, (temp32 >> 16) & 0xFFFF);
        runtime->instruction_pointer += 1;
        break;

    case GLOW_MULT_UINT32:
        temp64 = pop_operand16(runtime);
        temp64_2 = pop_operand16(runtime);
        temp64 = temp64 * temp64_2;
        push_operand32(runtime, temp64 & 0xFFFFFFFF);
        push_operand32(runtime, (temp64 >> 32) & 0xFFFFFFFF);
        runtime->instruction_pointer += 1;
        break;

    case GLOW_MULT_UINT64:
        temp64 = pop_operand64(runtime);
        temp64_2 = pop_operand64(runtime);
#ifdef ARCHITECTURE_X86_64
        asm("imul %%rbx" : "=a"(temp64), "=d"(temp64_2) : "a" (temp64), "b" (temp64_2));
#else
#error "instruction GLOW_MULT_UINT64 not yet implemented for x86 architecture."
#endif
        push_operand64(runtime, temp64);
        push_operand64(runtime, temp64_2);
        runtime->instruction_pointer += 1;

    case GLOW_MULT_FLOAT32:
        temp32 = pop_operand32(runtime);
        temp32_2 = pop_operand32(runtime);
        {
            glow_float32 res = *((glow_float32*) &temp32) * *((glow_float32*) &temp32_2);
            push_operand32(runtime, *(glow_uint32*) &res);
        }
        runtime->instruction_pointer += 1;
        break;

    case GLOW_MULT_FLOAT64:
        temp64 = pop_operand64(runtime);
        temp64_2 = pop_operand64(runtime);
        push_operand64(runtime, *(glow_uint64*)
                       ((glow_float64*) &temp64) * /* <- for mult, no
                                                    dereferencing ;D */
                       *((glow_float64*) &temp64_2));
        runtime->instruction_pointer += 1;
        break;

            
#define OPERATION_INT(tempvar1, tempvar2, datatype, pop_func, push_func, id, operator) \
    case id: \
    tempvar1 = (datatype) pop_func(runtime); \
    tempvar2 = (datatype) pop_func(runtime); \
    tempvar1 = tempvar2 operator tempvar1; \
    push_func(runtime, tempvar1); \
    runtime->instruction_pointer += 1; \
    break;
            
#define OPERATION_FLOAT32(id, operator) \
    case id: { \
    glow_uint32 temp = pop_operand32(runtime); \
    glow_uint32 temp2 = pop_operand32(runtime); \
    glow_float32 t = *((glow_float32*) &temp); \
    glow_float32 t2 = *((glow_float32*) &temp2); \
    t = t2 operator t; \
    push_operand32(runtime, *((glow_uint32*) &t)); \
    runtime->instruction_pointer += 1; \
    break; \
    }

#define OPERATION_FLOAT64(id, operator) \
    case id: { \
    glow_uint64 temp = pop_operand64(runtime); \
    glow_uint64 temp2 = pop_operand64(runtime); \
    glow_float64 t = *((glow_float64*) &temp); \
    glow_float64 t2 = *((glow_float64*) &temp2); \
    t = t2 operator t; \
    push_operand64(runtime, *((glow_uint64*) &t)); \
    runtime->instruction_pointer += 1; \
    break; \
    }

    /*case id: \
    tempvar1 = (datatype) pop_func(runtime); \
    tempvar2 = (datatype) pop_func(runtime); \
    tempvar1 = tempvar1 operator tempvar2; \
    push_func(runtime, tempvar1); \
    runtime->instruction_pointer += 1; \
    break;
    */
#define OPERATION_INT8(id, operator) \
OPERATION_INT(temps8, temps8_2, glow_int8, pop_operand8, push_operand8, id, operator)
            
#define OPERATION_INT16(id, operator) \
OPERATION_INT(temps16, temps16_2, glow_int16, pop_operand16, push_operand16, id, operator)
            
#define OPERATION_INT32(id, operator) \
OPERATION_INT(temps32, temps32_2, glow_int32, pop_operand32, push_operand32, id, operator)
            
#define OPERATION_INT64(id, operator) \
OPERATION_INT(temps64, temps64_2, glow_int64, pop_operand64, push_operand64, id, operator)
            
#define OPERATION_UINT8(id, operator) \
OPERATION_INT(temp8, temp8_2, glow_uint8, pop_operand8, push_operand8, id, operator)
            
#define OPERATION_UINT16(id, operator) \
OPERATION_INT(temp16, temp16_2, glow_uint16, pop_operand16, push_operand16, id, operator)
            
#define OPERATION_UINT32(id, operator) \
OPERATION_INT(temp32, temp32_2, glow_uint32, pop_operand32, push_operand32, id, operator)
            
#define OPERATION_UINT64(id, operator) \
OPERATION_INT(temp64, temp64_2, glow_uint64, pop_operand64, push_operand64, id, operator)


#define OPERATION_CONVERT(pop_func, push_func, pop_type, push_type, src_type, dest_type, id) \
    case id: { \
    pop_type pop = pop_func(runtime); \
    src_type temp = *((src_type*) &pop); \
    dest_type dest = (dest_type) temp; \
    push_type push = *((push_type*) &dest); \
    push_func(runtime, push); \
    runtime->instruction_pointer += 1; \
    break; \
    }


#define OPERATION_CONVERT_UINT(pop_func, push_func, src_type, dest_type, id) \
    OPERATION_CONVERT(pop_func, push_func, src_type, dest_type, src_type, dest_type, id)

        OPERATION_INT8(GLOW_DIV_INT8, /)
        OPERATION_INT16(GLOW_DIV_INT16, /)
        OPERATION_INT32(GLOW_DIV_INT32, /)
        OPERATION_INT64(GLOW_DIV_INT64, /)

        OPERATION_UINT8(GLOW_DIV_UINT8, /)
        OPERATION_UINT16(GLOW_DIV_UINT16, /)
        OPERATION_UINT32(GLOW_DIV_UINT32, /)
        OPERATION_UINT64(GLOW_DIV_UINT64, /)

        OPERATION_FLOAT32(GLOW_DIV_FLOAT32, /)
        OPERATION_FLOAT64(GLOW_DIV_FLOAT64, /)

        OPERATION_UINT8(GLOW_AND_INT8, &)
        OPERATION_UINT16(GLOW_AND_INT16, &)
        OPERATION_UINT32(GLOW_AND_INT32, &)
        OPERATION_UINT64(GLOW_AND_INT64, &)

        OPERATION_UINT8(GLOW_OR_INT8, |)
        OPERATION_UINT16(GLOW_OR_INT16, |)
        OPERATION_UINT32(GLOW_OR_INT32, |)
        OPERATION_UINT64(GLOW_OR_INT64, |)

        OPERATION_UINT8(GLOW_XOR_INT8, ^)
        OPERATION_UINT16(GLOW_XOR_INT16, ^)
        OPERATION_UINT32(GLOW_XOR_INT32, ^)
        OPERATION_UINT64(GLOW_XOR_INT64, ^)

        OPERATION_CONVERT(pop_operand8, push_operand32, glow_uint8, glow_uint32, glow_int8, glow_float32, GLOW_INT8_TO_FLOAT32)
        OPERATION_CONVERT(pop_operand16, push_operand32, glow_uint16, glow_uint32, glow_int16, glow_float32, GLOW_INT16_TO_FLOAT32)
        OPERATION_CONVERT(pop_operand32, push_operand32, glow_uint32, glow_uint32, glow_int32, glow_float32, GLOW_INT32_TO_FLOAT32)
        OPERATION_CONVERT(pop_operand64, push_operand32, glow_uint64, glow_uint32, glow_int64, glow_float32, GLOW_INT64_TO_FLOAT32)

        OPERATION_CONVERT(pop_operand8, push_operand64, glow_uint8, glow_uint64, glow_int8, glow_float64, GLOW_INT8_TO_FLOAT64)
        OPERATION_CONVERT(pop_operand16, push_operand64, glow_uint16, glow_uint64, glow_int16, glow_float64, GLOW_INT16_TO_FLOAT64)
        OPERATION_CONVERT(pop_operand32, push_operand64, glow_uint32, glow_uint64, glow_int32, glow_float64, GLOW_INT32_TO_FLOAT64)
        OPERATION_CONVERT(pop_operand64, push_operand64, glow_uint64, glow_uint64, glow_int64, glow_float64, GLOW_INT64_TO_FLOAT64)

        OPERATION_CONVERT(pop_operand32, push_operand8, glow_uint32, glow_uint8, glow_float32, glow_int8, GLOW_FLOAT32_TO_INT8)
        OPERATION_CONVERT(pop_operand32, push_operand16, glow_uint32, glow_uint16, glow_float32, glow_int16, GLOW_FLOAT32_TO_INT16)
        OPERATION_CONVERT(pop_operand32, push_operand32, glow_uint32, glow_uint32, glow_float32, glow_int32, GLOW_FLOAT32_TO_INT32)
        OPERATION_CONVERT(pop_operand32, push_operand64, glow_uint32, glow_uint64, glow_float32, glow_int64, GLOW_FLOAT32_TO_INT64)

        OPERATION_CONVERT(pop_operand64, push_operand8, glow_uint64, glow_uint8, glow_float64, glow_int8, GLOW_FLOAT64_TO_INT8)
        OPERATION_CONVERT(pop_operand64, push_operand16, glow_uint64, glow_uint16, glow_float64, glow_int16, GLOW_FLOAT64_TO_INT16)
        OPERATION_CONVERT(pop_operand64, push_operand32, glow_uint64, glow_uint32, glow_float64, glow_int32, GLOW_FLOAT64_TO_INT32)
        OPERATION_CONVERT(pop_operand64, push_operand64, glow_uint64, glow_uint64, glow_float64, glow_int64, GLOW_FLOAT64_TO_INT64)

        OPERATION_CONVERT_UINT(pop_operand8, push_operand64, glow_uint8, glow_uint64, GLOW_UINT8_TO_UINT64)
        OPERATION_CONVERT_UINT(pop_operand16, push_operand64, glow_uint16, glow_uint64, GLOW_UINT16_TO_UINT64)
        OPERATION_CONVERT_UINT(pop_operand32, push_operand64, glow_uint32, glow_uint64, GLOW_UINT32_TO_UINT64)

        OPERATION_CONVERT_UINT(pop_operand8, push_operand32, glow_uint8, glow_uint32, GLOW_UINT8_TO_UINT32)
        OPERATION_CONVERT_UINT(pop_operand16, push_operand32, glow_uint16, glow_uint32, GLOW_UINT16_TO_UINT32)
        OPERATION_CONVERT_UINT(pop_operand64, push_operand32, glow_uint64, glow_uint32, GLOW_UINT64_TO_UINT32)

        OPERATION_CONVERT_UINT(pop_operand8, push_operand16, glow_uint8, glow_uint16, GLOW_UINT8_TO_UINT16)
        OPERATION_CONVERT_UINT(pop_operand32, push_operand16, glow_uint32, glow_uint16, GLOW_UINT32_TO_UINT16)
        OPERATION_CONVERT_UINT(pop_operand64, push_operand16, glow_uint64, glow_uint16, GLOW_UINT64_TO_UINT16)

        OPERATION_CONVERT_UINT(pop_operand16, push_operand8, glow_uint16, glow_uint8, GLOW_UINT16_TO_UINT8)
        OPERATION_CONVERT_UINT(pop_operand32, push_operand8, glow_uint32, glow_uint8, GLOW_UINT32_TO_UINT8)
        OPERATION_CONVERT_UINT(pop_operand64, push_operand8, glow_uint64, glow_uint8, GLOW_UINT64_TO_UINT8)

        OPERATION_CONVERT(pop_operand32, push_operand64, glow_uint32, glow_uint64, glow_float32, glow_float64, GLOW_FLOAT32_TO_FLOAT64)
        OPERATION_CONVERT(pop_operand64, push_operand32, glow_uint64, glow_uint32, glow_float64, glow_float32, GLOW_FLOAT64_TO_FLOAT32)


    case GLOW_NOT_INT8:
        push_operand8(runtime, /*not*/~pop_operand8(runtime));
        runtime->instruction_pointer += 1;
        break;
    case GLOW_NOT_INT16:
        push_operand16(runtime, /*not*/~pop_operand16(runtime));
        runtime->instruction_pointer += 1;
        break;
    case GLOW_NOT_INT32:
        push_operand32(runtime, /*not*/~pop_operand32(runtime));
        runtime->instruction_pointer += 1;
        break;
    case GLOW_NOT_INT64:
        push_operand64(runtime, /*not*/~pop_operand64(runtime));
        runtime->instruction_pointer += 1;
        break;


        OPERATION_INT8(GLOW_MOD_INT8, %)
        OPERATION_INT16(GLOW_MOD_INT16, %)
        OPERATION_INT32(GLOW_MOD_INT32, %)
        OPERATION_INT64(GLOW_MOD_INT64, %)

        OPERATION_UINT8(GLOW_MOD_UINT8, %)
        OPERATION_UINT16(GLOW_MOD_UINT16, %)
        OPERATION_UINT32(GLOW_MOD_UINT32, %)
        OPERATION_UINT64(GLOW_MOD_UINT64, %)

        OPERATION_UINT8(GLOW_SUB_INT8, -)
        OPERATION_UINT16(GLOW_SUB_INT16, -)
        OPERATION_UINT32(GLOW_SUB_INT32, -)
        OPERATION_UINT64(GLOW_SUB_INT64, -)
        OPERATION_FLOAT32(GLOW_SUB_FLOAT32, -)
        OPERATION_FLOAT64(GLOW_SUB_FLOAT64, -)
;

    /*
     * calls
     */
            
            
    case GLOW_CALL_NATIVE:
        temp32 = *((glow_int32*) (runtime->instruction_pointer + 1));
        if (temp32 < runtime->native_function_count)
            temp64 = runtime->native_functions[temp32](runtime);
        else
            temp64 = 0;
        push_operand64(runtime, temp64);
        runtime->instruction_pointer += 5;
        break;

    case GLOW_EXIT:
        return;

    default:
        printf("error: unknown instruction: 0x%d\n", (int) val);
        return;
    }

    goto start;
}


static void push_operand8(glow_runtime_env* runtime_env, glow_uint8 byte)
{
    *((glow_uint8*) (runtime_env->operand_stack_top)) = byte;
    runtime_env->operand_stack_top += 1;
}


static void push_operand16(glow_runtime_env* runtime_env, glow_uint16 int16)
{
    *((glow_uint16*) (runtime_env->operand_stack_top)) = int16;
    runtime_env->operand_stack_top += 2;
}


static void push_operand32(glow_runtime_env* runtime_env, glow_uint32 int32)
{
    *((glow_uint32*) (runtime_env->operand_stack_top)) = int32;
    runtime_env->operand_stack_top += 4;
}


static void push_operand64(glow_runtime_env* runtime_env, glow_uint64 int64)
{
    *((glow_uint64*) (runtime_env->operand_stack_top)) = int64;
    runtime_env->operand_stack_top += 8;
}


static void push_operand_word(glow_runtime_env* runtime_env, glow_word word)
{
    *((glow_word*) (runtime_env->operand_stack_top)) = word;
    runtime_env->operand_stack_top += sizeof word;
}


static void push_operand_f32(glow_runtime_env* runtime_env, glow_float32 flo)
{
    push_operand32(runtime_env, *((glow_uint32*) &flo));
}


static void push_operand_f64(glow_runtime_env* runtime_env, glow_float64 flo)
{
    push_operand64(runtime_env, *((glow_uint64*) &flo));
}


static glow_uint8 pop_operand8(glow_runtime_env* runtime_env)
{
    runtime_env->operand_stack_top -= 1;
    return *((glow_uint8*) (runtime_env->operand_stack_top));
}


static glow_uint16 pop_operand16(glow_runtime_env* runtime_env)
{
    runtime_env->operand_stack_top -= 2;
    return *((glow_uint16*) (runtime_env->operand_stack_top));
}


static glow_uint32 pop_operand32(glow_runtime_env* runtime_env)
{
    runtime_env->operand_stack_top -= 4;
    return *((glow_uint32*) (runtime_env->operand_stack_top));
}


static glow_uint64 pop_operand64(glow_runtime_env* runtime_env)
{
    runtime_env->operand_stack_top -= 8;
    return *((glow_uint64*) (runtime_env->operand_stack_top));
}


static glow_word pop_operand_word(glow_runtime_env* runtime_env)
{
    runtime_env->operand_stack_top -= sizeof(glow_word);
    return *((glow_word*) (runtime_env->operand_stack_top));
}


static glow_float32 pop_operand_f32(glow_runtime_env* runtime_env)
{
    glow_uint32 pop = pop_operand32(runtime_env);
    return *((glow_float32*) &pop);
}


static glow_float64 pop_operand_f64(glow_runtime_env* runtime_env)
{
    glow_uint64 pop = pop_operand64(runtime_env);
    return *((glow_float64*) &pop);
}

