#include "conversion_table.h"

static struct glow_conversion_entry conversion_table[] =
{

{"nop",     GLOW_NO_OPERATION, "does nothing"},
{"loadc8",  GLOW_LOAD_CONSTANT_INT8, "loads a 8-bit constant on the stack"},
{"loadc16", GLOW_LOAD_CONSTANT_INT16, "loads a 16-bit constant on the stack"},
{"loadc32", GLOW_LOAD_CONSTANT_INT32, "loads a 32-bit constant on the stack"},
{"loadc64", GLOW_LOAD_CONSTANT_INT64, "loads a 64-bit constant on the stack"},

{"loadv8",  GLOW_LOAD_LOCAL_INT8, "loads a 8-bit variable on the stack; Takes a 32-bit offset as argument"},
{"loadv16", GLOW_LOAD_LOCAL_INT16, "loads a 16-bit variable on the stack; Takes a 32-bit offset as argument"},
{"loadv32", GLOW_LOAD_LOCAL_INT32, "loads a 32-bit variable on the stack; Takes a 32-bit offset as argument"},
{"loadv64", GLOW_LOAD_LOCAL_INT64, "loads a 64-bit variable on the stack; Takes a 32-bit offset as argument"},
{"loadvref",GLOW_LOAD_LOCAL_REFERENCE, "loads a variable reference on the stack; Takes a 32-bit offset as argument"},

{"storev8", GLOW_STORE_LOCAL_INT8},
{"storev16",GLOW_STORE_LOCAL_INT16},
{"storev32",GLOW_STORE_LOCAL_INT32},
{"storev64",GLOW_STORE_LOCAL_INT64},
{"storevref",GLOW_STORE_LOCAL_REFERENCE},

{"pop8",    GLOW_POP_8, "deletes an 8-bit value from the top of the stack"},
{"pop16",   GLOW_POP_16, "deletes an 16-bit value from the top of the stack"},
{"pop32",   GLOW_POP_32, "deletes an 32-bit value from the top of the stack"},
{"pop64",   GLOW_POP_64, "deletes an 64-bit value from the top of the stack"},

{"cmp8",    GLOW_COMPARE_INT8},
{"cmp16",   GLOW_COMPARE_INT16},
{"cmp32",   GLOW_COMPARE_INT32},
{"cmp64",   GLOW_COMPARE_INT64},
{"cmpu8",   GLOW_COMPARE_UINT8},
{"cmpu16",  GLOW_COMPARE_UINT16},
{"cmpu32",  GLOW_COMPARE_UINT32},
{"cmpu64",  GLOW_COMPARE_UINT64},
{"cmpf32",  GLOW_COMPARE_FLOAT32},
{"cmpf64",  GLOW_COMPARE_FLOAT64},

{"jmp",     GLOW_JUMP},
{"jz",      GLOW_JUMP_IF_ZERO},
{"je",      GLOW_JUMP_IF_EQUAL},
{"jnz",     GLOW_JUMP_IF_NOT_ZERO},
{"jne",     GLOW_JUMP_IF_NOT_EQUAL},
{"jg",      GLOW_JUMP_IF_GREATER},
{"jl",      GLOW_JUMP_IF_LESS},
{"jge",     GLOW_JUMP_IF_GREATER_OR_EQUAL},
{"jle",     GLOW_JUMP_IF_LESS_OR_EQUAL},

{"se8_16",  GLOW_SIGN_EXTEND_INT8_TO_INT16},
{"se8_32",  GLOW_SIGN_EXTEND_INT8_TO_INT32},
{"se8_64",  GLOW_SIGN_EXTEND_INT8_TO_INT64},
{"se16_32", GLOW_SIGN_EXTEND_INT16_TO_INT32},
{"se16_64", GLOW_SIGN_EXTEND_INT16_TO_INT64},
{"se32_64", GLOW_SIGN_EXTEND_INT32_TO_INT64},

{"neg8",    GLOW_NEGATE_INT8},
{"neg16",   GLOW_NEGATE_INT16},
{"neg32",   GLOW_NEGATE_INT32},
{"neg64",   GLOW_NEGATE_INT64},
{"negf32",  GLOW_NEGATE_FLOAT32},
{"negf64",  GLOW_NEGATE_FLOAT64},

{"dup8",    GLOW_DUPLICATE_8},
{"dup16",   GLOW_DUPLICATE_16},
{"dup32",   GLOW_DUPLICATE_32},
{"dup64",   GLOW_DUPLICATE_64},

{"add8",    GLOW_ADD_INT8},
{"add16",   GLOW_ADD_INT16},
{"add32",   GLOW_ADD_INT32},
{"add64",   GLOW_ADD_INT64},
{"addf32",  GLOW_ADD_FLOAT32},
{"addf64",  GLOW_ADD_FLOAT64},

{"mul8",    GLOW_MULT_INT8},
{"mul16",   GLOW_MULT_INT16},
{"mul32",   GLOW_MULT_INT32},
{"mul64",   GLOW_MULT_INT64},
{"mulu8",   GLOW_MULT_UINT8},
{"mulu16",  GLOW_MULT_UINT16},
{"mulu32",  GLOW_MULT_UINT32},
{"mulu64",  GLOW_MULT_UINT64},
{"mulf32",  GLOW_MULT_FLOAT32},
{"mulf64",  GLOW_MULT_FLOAT64},

{"div8",    GLOW_DIV_INT8},
{"div16",   GLOW_DIV_INT16},
{"div32",   GLOW_DIV_INT32},
{"div64",   GLOW_DIV_INT64},
{"divu8",   GLOW_DIV_UINT8},
{"divu16",  GLOW_DIV_UINT16},
{"divu32",  GLOW_DIV_UINT32},
{"divu64",  GLOW_DIV_UINT64},
{"divf32",  GLOW_DIV_FLOAT32},
{"divf64",  GLOW_DIV_FLOAT64},

{"and8",    GLOW_AND_INT8},
{"and16",   GLOW_AND_INT16},
{"and32",   GLOW_AND_INT32},
{"and64",   GLOW_AND_INT64},

{"or8",     GLOW_OR_INT8},
{"or16",    GLOW_OR_INT16},
{"or32",    GLOW_OR_INT32},
{"or64",    GLOW_OR_INT64},

{"xor8",    GLOW_XOR_INT8},
{"xor16",   GLOW_XOR_INT16},
{"xor32",   GLOW_XOR_INT32},
{"xor64",   GLOW_XOR_INT64},

{"not8",    GLOW_NOT_INT8},
{"not16",   GLOW_NOT_INT16},
{"not32",   GLOW_NOT_INT32},
{"not64",   GLOW_NOT_INT64},

{"i8_f32",  GLOW_INT8_TO_FLOAT32},
{"i16_f32", GLOW_INT16_TO_FLOAT32},
{"i32_f32", GLOW_INT32_TO_FLOAT32},
{"i64_f32", GLOW_INT64_TO_FLOAT32},

{"i8_f64",  GLOW_INT8_TO_FLOAT64},
{"i16_f64", GLOW_INT16_TO_FLOAT64},
{"i32_f64", GLOW_INT32_TO_FLOAT64},
{"i64_f64", GLOW_INT64_TO_FLOAT64},

{"f32_i8",  GLOW_FLOAT32_TO_INT8},
{"f32_i16", GLOW_FLOAT32_TO_INT16},
{"f32_i32", GLOW_FLOAT32_TO_INT32},
{"f32_i64", GLOW_FLOAT32_TO_INT64},

{"f64_i8",  GLOW_FLOAT64_TO_INT8},
{"f64_i16", GLOW_FLOAT64_TO_INT16},
{"f64_i32", GLOW_FLOAT64_TO_INT32},
{"f64_i64", GLOW_FLOAT64_TO_INT64},

{"ui8_ui64",GLOW_UINT8_TO_UINT64},
{"ui16_ui64",GLOW_UINT16_TO_UINT64},
{"ui32_ui64",GLOW_UINT32_TO_UINT64},

{"ui8_ui32",GLOW_UINT8_TO_UINT32},
{"ui16_ui32",GLOW_UINT16_TO_UINT32},
{"ui64_ui64",GLOW_UINT64_TO_UINT32},

{"ui8_ui16",GLOW_UINT8_TO_UINT16},
{"ui32_ui16",GLOW_UINT32_TO_UINT16},
{"ui64_ui16",GLOW_UINT64_TO_UINT16},

{"ui16_ui8",GLOW_UINT16_TO_UINT8},
{"ui32_ui8",GLOW_UINT32_TO_UINT8},
{"ui64_ui8",GLOW_UINT64_TO_UINT8},

{"f32_f64", GLOW_FLOAT32_TO_FLOAT64},
{"f64_f32", GLOW_FLOAT64_TO_FLOAT32},

{"mod8",    GLOW_MOD_INT8},
{"mod16",   GLOW_MOD_INT16},
{"mod32",   GLOW_MOD_INT32},
{"mod64",   GLOW_MOD_INT64},

{"modu8",   GLOW_MOD_UINT8},
{"modu16",  GLOW_MOD_UINT16},
{"modu32",  GLOW_MOD_UINT32},
{"modu64",  GLOW_MOD_UINT64},

{"sub8",    GLOW_SUB_INT8},
{"sub16",   GLOW_SUB_INT16},
{"sub32",   GLOW_SUB_INT32},
{"sub64",   GLOW_SUB_INT64},
{"subf32",  GLOW_SUB_FLOAT32},
{"subf64",  GLOW_SUB_FLOAT64},

{"new", GLOW_ALLOCATE_OBJECT},
{"rem", GLOW_DELETE_OBJECT},

{"loadf8", GLOW_LOAD_FROM_FIELD_8},
{"loadf16", GLOW_LOAD_FROM_FIELD_16},
{"loadf32", GLOW_LOAD_FROM_FIELD_32},
{"loadf64", GLOW_LOAD_FROM_FIELD_64},
{"loadfref", GLOW_LOAD_FROM_FIELD_REFERENCE},

{"storef8", GLOW_STORE_IN_FIELD_8},
{"storef16", GLOW_STORE_IN_FIELD_16},
{"storef32", GLOW_STORE_IN_FIELD_32},
{"storef64", GLOW_STORE_IN_FIELD_64},
{"storefref", GLOW_STORE_IN_FIELD_REFERENCE},

{"calln",   GLOW_CALL_NATIVE},

{"exit",    GLOW_EXIT},
};

struct glow_conversion_entry *glow_conversion_table = conversion_table;


const int glow_instruction_table_count = sizeof(conversion_table) / sizeof(glow_conversion_table[0]);



/*!
 * \brief compares two strings ignoring the case
 *
 * Each character is compared. If two differ, they must be upper-case
 * and lower-case version of the same letter. If they are not either,
 * the function returns 0
 *
 * \param a the first string
 * \param b the second string
 * \return 1 if they are equal (ignoring case), 0 if they are different
 */
int equal_ignore_case(const char* a, const char* b);


glow_instruction glow_lookup_instruction(const char* token)
{
    for (int i = glow_instruction_table_count - 1; i >= 0; i--)
    {
        if (equal_ignore_case(glow_conversion_table[i].token, token)) {
            return glow_conversion_table[i].operation;
        }
    }

    return 0xFFFFFFFF;
}


const char* glow_print_assembly(glow_instruction instr)
{
    for (int i = glow_instruction_table_count - 1; i >= 0; i--)
    {
        if (glow_conversion_table[i].operation == instr) {
            return glow_conversion_table[i].token;
        }
    }

    return 0;
}


void glow_print_reference(FILE* stream)
{
    // fprintf(stream, "%d", glow_instruction_table_count);
    for (int i = 0; i < glow_instruction_table_count; i++)
    {
        const char* instr = glow_conversion_table[i].token;
        glow_instruction opcode = glow_conversion_table[i].operation;
        const char* description = glow_conversion_table[i].description;

        char opcode_str[9];
        if (opcode < 0x10)
            sprintf(opcode_str, "0%X", opcode);
        else
            sprintf(opcode_str, "%X", opcode);

        if (description != 0)
            fprintf(stream, "%-12s opcode 0x%-8s %s\n", instr, opcode_str, description);
        else
            fprintf(stream, "%-12s opcode 0x%-8s\n", instr, opcode_str);
    }
}


int equal_ignore_case(const char* a, const char* b)
{
    while (*a != 0 && *b != 0) {
        if (*a != *b) {
            if (*a > 'A' && *a < 'Z') {
                if (*b != *a + ('a' - 'A'))
                    return 0;
            } else if (*a > 'a' && *a < 'z') {
                if (*a != *b + ('a' - 'A'))
                    return 0;
            } else {
                return 0;
            }
        }
        a++;
        b++;
    }
    return 1;
}

