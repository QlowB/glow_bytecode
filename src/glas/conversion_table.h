#include "../bytecode.h"
#include <stdio.h>

extern struct glow_conversion_entry
{
    const char* token;
    glow_instruction operation;
    const char* description;
} *glow_conversion_table;


extern const int glow_instruction_table_count;


/*!
 * \brief returns the instruction associated with this token
 * \param token
 * \return the instruction associated with this token,
 *         or 0xFFFFFFFF on error
 */
glow_instruction glow_lookup_instruction(const char* token);

/*!
 * \brief returns the assembly code of a specific instruction
 * \param instr the instruction
 * \return the assembly operation as a string
 */
const char* glow_print_assembly(glow_instruction instr);


/*!
 * \brief prints all commands to a stream
 * \param stream
 */
void glow_print_reference(FILE* stream);
