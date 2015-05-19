#ifndef GLOW_INSTRUCTIONCOMPILER_H_
#define GLOW_INSTRUCTIONCOMPILER_H_

#include "bytecode_assembler.h"

#include "../bytecode.h"
#include "../integers.h"

struct glow_asm_inst_;


/*!
 * \brief compiles a single-byte instruction with an argument
 * \param buf the buffer to write to
 * \param oper
 * \param inst the instruction to compile
 * \param argument_bytes number of bytes used to store the argument
 * \return
 */
int glow_compile_single_int(char* buf, glow_instruction oper,
                            const struct glow_asm_inst_* inst,
                            int argument_bytes);

/*!
 * \brief compiles a single byte instruction
 * \param buf the buffer to write to
 * \param oper the operation to compile
 * \return -1 on fail, 1 otherwise
 */
int glow_compile_single(char* buf, glow_instruction oper);

/*!
 * \brief compiles a jump instruction
 * \param block the block to write to
 * \param oper the operation to compile
 * \param inst the instruction to compile
 * \param mark a mark
 * \return -1 on fail, number of bytes written otherwise
 */
int glow_compile_jump(char* buf, glow_bytecode_block* block, glow_instruction oper,
                      const struct glow_asm_inst_* inst);


#endif // GLOW_INSTRUCTIONCOMPILER_H_
