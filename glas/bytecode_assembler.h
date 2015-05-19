#ifndef GLOW_BYTECODEASSEMBLER_H_
#define GLOW_BYTECODEASSEMBLER_H_

#include <stdio.h>
#include "../bytecode.h"

/*!
 * defines the maximum number of bytes that can possibly represent one single
 * instruction. If a byte array and an instruction is provided to the compiler,
 * it can be assumed that the array doesn't have to be longer than this value.
 */
#define GLOW_MAX_BYTES_PER_INSTRUCTION 32


#define GLOW_FATAL_ERROR_MESSAGE "internal error"

#define GLOW_IDENTIFIER 0
#define GLOW_FLOAT 1
#define GLOW_INTEGER 2
#define GLOW_HEX_INTEGER 3


extern char glow_error_message[1024];

typedef struct
{
    /*!
     * \brief name of the jump mark
     */
    const char* name;

    /*!
     * \brief where it points to (in the code)
     */
    long where;
} glow_jump_mark;


typedef struct
{
    const char* name;

    /*!
     * \brief where the jump has to be linked
     */
    long link_pos;

    /*!
     * \brief from where the jum starts (normally == where + 4)
     */
    long from_where;
} glow_jump;

typedef struct
{
    char* buffer;
    int used_size;
    int buffer_size;

    /*!
     * \brief marks that can be jumped to
     */
    glow_jump_mark* jump_marks;
    int jump_marks_count;
    int jump_marks_used_count;

    /*!
     * \brief actual jumps, that must be linked
     */
    glow_jump* jumps;
    int jumps_count;
    int jumps_used_count;
} glow_bytecode_block;


/*!
 * if it is a jump mark, it's name is stored in operation
 */
typedef struct glow_asm_inst_
{
    int is_jump_mark;
    char* operation;
    int operand_1_type;
    char* operand_1;
} glow_assembler_instruction;


/*!
 * \brief must be called before writing the program; fills in jump marks
 */
int glow_link(glow_bytecode_block *program);

/*!
 * \brief initializes a glow bytecode block
 */
void glow_init_block(glow_bytecode_block* block);


/*!
 * \brief deletes a glow bytecode block
 */
void glow_destroy_block(glow_bytecode_block* block);


/*!
 * \brief initializes a glow bytecode assembly instruction
 */
void glow_init_instruction(glow_assembler_instruction* inst);


/*!
 * \brief converts an operand to an integer
 * the operand must be an integer or a hex integer or a 64-bit floating point value
 * \param operand the index of the operand
 * \return the integer value
 */
long long glow_get_integer_operand64(const glow_assembler_instruction* inst, int operand);


/*!
 * \brief adds some bytes to the bytecode buffer in the block
 */
void glow_add_bytecode(glow_bytecode_block* block, const char* bytes, int byte_count);


void glow_add_jump_mark(glow_bytecode_block* block, const char* name, long where);


void glow_add_jump(glow_bytecode_block* block, const char* name, long link_pos, long from_where);


/*!
 * \brief compiles one instruction and adds it to the block
 *
 * \param inst the assembly data to compile
 * \return 0 if succeeded, nonzero otherwise
 */
int glow_compile_instruction(glow_bytecode_block* block, glow_assembler_instruction* inst);


/*!
 * saves a glow bytecode file (*.glob format) to the stream
 */
void glow_save_glob(glow_bytecode_block* block, FILE* stream);


/*!
 * reads a glow bytecode file (*.glob format) from the stream
 */
void glow_load_glob(glow_bytecode_block* block, FILE* stream);


/*!
 * \brief prints the last error
 * \param err_msg the buffer to write the error message to
 * \param buffer_size the size of the specified buffer
 */
void glow_get_last_error(char* err_msg, int buffer_size);


#endif // GLOW_BYTECODEASSEMBLER_H_
