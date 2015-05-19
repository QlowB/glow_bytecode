#ifndef GLOW_BYTECODEINTERPRETER_H_
#define GLOW_BYTECODEINTERPRETER_H_

#include "integers.h"

static const int glow_operand_stack_size = 256;
static const int glow_call_stack_size = 1024 * 1024;

struct glow_runtime_env_struct_;
typedef struct glow_runtime_env_struct_ glow_runtime_env;

typedef glow_int64 (*glow_native_func) (glow_runtime_env*);

/*!
 * data structure describing the current state of the interpreter.
 */
struct glow_runtime_env_struct_
{
    /*! pointer to the base of the operand stack */
    void* operand_stack_base;

    /*! pointer to the first free element on the operand stack */
    void* operand_stack_top;

    /*! base pointer of the call stack */
    void* call_stack_base;

    /*! pointer to the first free entry on the call stack */
    void* call_stack_top;

    /*! pointer to the current instruction */
    unsigned char* instruction_pointer;

    /*! pointer to the previously allocated instruction memory */
    char* instruction_memory;

    /*! number of native functions in this environment */
    int native_function_count;

    /*! array containing the native functions */
    glow_native_func* native_functions;
};


void glow_init_runtime(glow_runtime_env* runtime, void* instruction_memory);
void glow_destroy_runtime(glow_runtime_env* runtime);


/*!
 * \brief processes one glow operation
 *
 * executes the next instruction in the byte code structure
 */
void glow_process_instructions(glow_runtime_env *runtime);


#endif // GLOW_BYTECODEINTERPRETER_H_
