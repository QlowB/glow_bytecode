#ifndef GLOW_BYTECODE_H_
#define GLOW_BYTECODE_H_

/*!
 * \brief instruction byte codes
 *
 * Each instruction is one byte long. Only if the upper nibble equals
 * hex F (binary 1111), the instruction is two bytes long.
 */
typedef enum {
    /*!
     * \brief as the name says, does nothing
     */
    GLOW_NO_OPERATION = 0x00,

    /*!
     * pushes a one-byte constant (following this instruction)
     * on the execution stack
     */
    GLOW_LOAD_CONSTANT_INT8 = 0x01,

    /*!
     * pushes a two-byte constant (following this instruction)
     * on the execution stack
     */
    GLOW_LOAD_CONSTANT_INT16 = 0x02,

    /*!
     * pushes a four-byte constant (following this instruction)
     * on the execution stack
     *
     * \note 32-bit floating point numbers can also be considered four-byte constants
     */
    GLOW_LOAD_CONSTANT_INT32 = 0x03,

    /*!
     * pushes a eight-byte constant (following this instruction)
     * on the execution stack
     *
     * \note 64-bit floating point numbers can also be considered eight-byte constants
     */
    GLOW_LOAD_CONSTANT_INT64 = 0x04,

    GLOW_JUMP = 0x08,

    /*!
     * pushes a one-byte local variable (stack offset in 4 bytes following this instruction)
     * on the execution stack
     */
    GLOW_LOAD_LOCAL_INT8 = 0x10,

    /*!
     * pushes a two-byte local variable (stack offset in 4 bytes following this instruction)
     * on the execution stack
     */
    GLOW_LOAD_LOCAL_INT16 = 0x11,

    /*!
     * pushes a four-byte local variable (stack offset in 4 bytes following this instruction)
     * on the execution stack
     *
     * \note 32-bit floating point numbers can also be considered four-byte constants
     */
    GLOW_LOAD_LOCAL_INT32 = 0x12,

    /*!
     * pushes a eight-byte local variable (stack offset in 4 bytes following this instruction)
     * on the execution stack
     *
     * \note 64-bit floating point numbers can also be considered eight-byte constants
     */
    GLOW_LOAD_LOCAL_INT64 = 0x13,
    
    /*!
     * pushes a word-size local variable (stack offset in 4 bytes following this instruction)
     * on the execution stack
     */
    GLOW_LOAD_LOCAL_REFERENCE = 0x18,

    /*!
     * saves the topmost one-byte operand variable in the call stack
     * (stack offset in 4 bytes following this instruction)
     */
    GLOW_STORE_LOCAL_INT8 = 0x14,

    /*!
     * saves the topmost two-byte operand variable in the call stack
     * (stack offset in 4 bytes following this instruction)
     */
    GLOW_STORE_LOCAL_INT16 = 0x15,

    /*!
     * saves the topmost four-byte operand variable in the call stack
     * (stack offset in 4 bytes following this instruction)
     *
     * \note 32-bit floating point numbers can also be considered four-byte constants
     */
    GLOW_STORE_LOCAL_INT32 = 0x16,

    /*!
     * saves the topmost eight-byte operand variable in the call stack
     * (stack offset in 4 bytes following this instruction)
     *
     * \note 64-bit floating point numbers can also be considered eight-byte constants
     */
    GLOW_STORE_LOCAL_INT64 = 0x17,
    
    /*!
     * pushes a word-size local variable (stack offset in 4 bytes following this instruction)
     * on the execution stack
     */
    GLOW_STORE_LOCAL_REFERENCE = 0x19,

    /*!
     * pops an 8-bit value from the operand stack and deletes it.
     */
    GLOW_POP_8 = 0x1A,

    /*!
     * pops a 16-bit value from the operand stack and deletes it.
     */
    GLOW_POP_16 = 0x1B,

    /*
     * pops a 32-bit value from the operand stack and deletes it.
     */
    GLOW_POP_32 = 0x1C,

    /*
     * pops a 64-bit value from the operand stack and deletes it.
     */
    GLOW_POP_64 = 0x1D,

    /*!
     * pops two 8-bit integers from the operand stack and pushes the comparison result as
     * an 8-bit integer:
     * 1, if the integer on top was less than the other, -1 if it was the other way around
     * and 0 if they are equal.
     */
    GLOW_COMPARE_INT8 = 0x20,

    /*!
     * pops two 16-bit integers from the operand stack and pushes the comparison result as
     * an 8-bit integer:
     * 1, if the integer on top was less than the other, -1 if it was the other way around
     * and 0 if they are equal.
     */
    GLOW_COMPARE_INT16 = 0x21,

    /*!
     * pops two 32-bit integers from the operand stack and pushes the comparison result as
     * an 8-bit integer:
     * 1, if the integer on top was less than the other, -1 if it was the other way around
     * and 0 if they are equal.
     */
    GLOW_COMPARE_INT32 = 0x22,

    /*!
     * pops two 64-bit integers from the operand stack and pushes the comparison result as
     * an 8-bit integer:
     * 1, if the integer on top was less than the other, -1 if it was the other way around
     * and 0 if they are equal.
     */
    GLOW_COMPARE_INT64 = 0x23,


    GLOW_COMPARE_UINT8 = 0x24,
    GLOW_COMPARE_UINT16 = 0x25,
    GLOW_COMPARE_UINT32 = 0x26,
    GLOW_COMPARE_UINT64 = 0x27,
    
    GLOW_COMPARE_FLOAT32 = 0x28,
    
    GLOW_COMPARE_FLOAT64 = 0x29,

    /*!
     * \brief jumps to a relative location stored in the next four bytes.
     *
     * pops one byte from the operand stack; finishes execution of this instruction,
     * then adds the 32-bit relative jump address to the instruction pointer, only if the
     * popped byte equals zero
     */
    GLOW_JUMP_IF_ZERO = 0x2A,
    GLOW_JUMP_IF_EQUAL = 0x2A,
    
    /*!
     * \brief jumps to a relative location stored in the next four bytes.
     *
     * pops one byte from the operand stack; finishes execution of this instruction,
     * then adds the 32-bit relative jump address to the instruction pointer, only if the
     * popped byte isn't zero
     */
    GLOW_JUMP_IF_NOT_EQUAL = 0x2B,
    GLOW_JUMP_IF_NOT_ZERO = 0x2B,

    GLOW_JUMP_IF_GREATER = 0x2C,
    GLOW_JUMP_IF_LESS = 0x2D,
    GLOW_JUMP_IF_GREATER_OR_EQUAL = 0x2E,
    GLOW_JUMP_IF_LESS_OR_EQUAL = 0x2F,

    
    /*!
     * pops and sign extends a 8-bit integer to a 16-bit integer and pushes it again
     */
    GLOW_SIGN_EXTEND_INT8_TO_INT16 = 0x30,
    
    /*!
     * pops and sign extends a 8-bit integer to a 32-bit integer and pushes it again
     */
    GLOW_SIGN_EXTEND_INT8_TO_INT32 = 0x31,
    
    /*!
     * pops and sign extends a 8-bit integer to a 64-bit integer and pushes it again
     */
    GLOW_SIGN_EXTEND_INT8_TO_INT64 = 0x32,
    
    /*!
     * pops and sign extends a 16-bit integer to a 32-bit integer and pushes it again
     */
    GLOW_SIGN_EXTEND_INT16_TO_INT32 = 0x33,
    
    /*!
     * pops and sign extends a 16-bit integer to a 64-bit integer and pushes it again
     */
    GLOW_SIGN_EXTEND_INT16_TO_INT64 = 0x34,
    
    /*!
     * pops and sign extends a 32-bit integer to a -bit integer and pushes it again
     */
    GLOW_SIGN_EXTEND_INT32_TO_INT64 = 0x35,
    
    GLOW_NEGATE_INT8 = 0x36,
    GLOW_NEGATE_INT16 = 0x37,
    GLOW_NEGATE_INT32 = 0x38,
    GLOW_NEGATE_INT64 = 0x39,
    
    GLOW_NEGATE_FLOAT32 = 0x3A,
    GLOW_NEGATE_FLOAT64 = 0x3B,

    GLOW_DUPLICATE_8 = 0x3C,
    GLOW_DUPLICATE_16 = 0x3D,
    GLOW_DUPLICATE_32 = 0x3E,
    GLOW_DUPLICATE_64 = 0x3F,

    /*!
     * pops two 8-bit integers from the operands stack, adds them together, and pushes them
     * again on the stack.
     */
    GLOW_ADD_INT8 = 0x40,
    
    /*!
     * pops two 16-bit integers from the operands stack, adds them together, and pushes them
     * again on the stack.
     */
    GLOW_ADD_INT16 = 0x41,
    
    /*!
     * pops two 32-bit integers from the operands stack, adds them together, and pushes them
     * again on the stack.
     */
    GLOW_ADD_INT32 = 0x42,
    
    /*!
     * pops two 64-bit integers from the operands stack, adds them together, and pushes them
     * again on the stack.
     */
    GLOW_ADD_INT64 = 0x43,
    
    /*!
     * pops two 32-bit floats from the operands stack, adds them together, and pushes them
     * again on the stack.
     */
    GLOW_ADD_FLOAT32 = 0x44,
    
    /*!
     * pops two 64-bit floats from the operands stack, adds them together, and pushes them
     * again on the stack.
     */
    GLOW_ADD_FLOAT64 = 0x45,

    /*!
     * multiplying two integers of a specific length produces two outputs of the same length
     * on the stack again: on the bottom the result, and on top the overflow.
     */
    GLOW_MULT_INT8 = 0x48,
    GLOW_MULT_INT16 = 0x49,
    GLOW_MULT_INT32 = 0x4A,
    GLOW_MULT_INT64 = 0x4B,

    GLOW_MULT_UINT8 = 0x4C,
    GLOW_MULT_UINT16 = 0x4D,
    GLOW_MULT_UINT32 = 0x4E,
    GLOW_MULT_UINT64 = 0x4F,

    GLOW_MULT_FLOAT32 = 0x50,
    GLOW_MULT_FLOAT64 = 0x51,

    GLOW_DIV_INT8 = 0x58,
    GLOW_DIV_INT16 = 0x59,
    GLOW_DIV_INT32 = 0x5A,
    GLOW_DIV_INT64 = 0x5B,

    GLOW_DIV_UINT8 = 0x5C,
    GLOW_DIV_UINT16 = 0x5D,
    GLOW_DIV_UINT32 = 0x5E,
    GLOW_DIV_UINT64 = 0x5F,

    GLOW_DIV_FLOAT32 = 0x60,
    GLOW_DIV_FLOAT64 = 0x61,

    GLOW_AND_INT8 = 0x62,
    GLOW_AND_INT16 = 0x63,
    GLOW_AND_INT32 = 0x64,
    GLOW_AND_INT64 = 0x65,

    GLOW_OR_INT8 = 0x66,
    GLOW_OR_INT16 = 0x67,
    GLOW_OR_INT32 = 0x68,
    GLOW_OR_INT64 = 0x69,

    GLOW_XOR_INT8 = 0x6A,
    GLOW_XOR_INT16 = 0x6B,
    GLOW_XOR_INT32 = 0x6C,
    GLOW_XOR_INT64 = 0x6D,

    /*
     * invert integers
     */
    GLOW_NOT_INT8 = 0x6E,
    GLOW_NOT_INT16 = 0x6F,
    GLOW_NOT_INT32 = 0x70,
    GLOW_NOT_INT64 = 0x71,

    GLOW_INT8_TO_FLOAT32 = 0x80,
    GLOW_INT16_TO_FLOAT32 = 0x81,
    GLOW_INT32_TO_FLOAT32 = 0x82,
    GLOW_INT64_TO_FLOAT32 = 0x83,

    GLOW_INT8_TO_FLOAT64 = 0x84,
    GLOW_INT16_TO_FLOAT64 = 0x85,
    GLOW_INT32_TO_FLOAT64 = 0x86,
    GLOW_INT64_TO_FLOAT64 = 0x87,

    GLOW_FLOAT32_TO_INT8 = 0x88,
    GLOW_FLOAT32_TO_INT16 = 0x89,
    GLOW_FLOAT32_TO_INT32 = 0x8A,
    GLOW_FLOAT32_TO_INT64 = 0x8B,
    
    GLOW_FLOAT64_TO_INT8 = 0x8C,
    GLOW_FLOAT64_TO_INT16 = 0x8D,
    GLOW_FLOAT64_TO_INT32 = 0x8E,
    GLOW_FLOAT64_TO_INT64 = 0x8F,

    GLOW_UINT8_TO_UINT64 = 0x90,
    GLOW_UINT16_TO_UINT64 = 0x91,
    GLOW_UINT32_TO_UINT64 = 0x92,
    
    GLOW_UINT8_TO_UINT32 = 0x93,
    GLOW_UINT16_TO_UINT32 = 0x94,
    GLOW_UINT64_TO_UINT32 = 0x95,
    
    GLOW_UINT8_TO_UINT16 = 0x96,
    GLOW_UINT32_TO_UINT16 = 0x97,
    GLOW_UINT64_TO_UINT16 = 0x98,
    
    GLOW_UINT16_TO_UINT8 = 0x99,
    GLOW_UINT32_TO_UINT8 = 0x9A,
    GLOW_UINT64_TO_UINT8 = 0x9B,

    GLOW_FLOAT32_TO_FLOAT64 = 0x9C,
    GLOW_FLOAT64_TO_FLOAT32 = 0x9D,

    GLOW_MOD_INT8 = 0x9E, 
    GLOW_MOD_INT16 = 0x9F,
    GLOW_MOD_INT32 = 0xA0, 
    GLOW_MOD_INT64 = 0xA1,

    GLOW_MOD_UINT8 = 0xA2,
    GLOW_MOD_UINT16 = 0xA3,
    GLOW_MOD_UINT32 = 0xA4,
    GLOW_MOD_UINT64 = 0xA5,

    GLOW_SUB_INT8 = 0xA6,
    GLOW_SUB_INT16 = 0xA7,
    GLOW_SUB_INT32 = 0xA8,
    GLOW_SUB_INT64 = 0xA9,
    GLOW_SUB_FLOAT32 = 0xAA,
    GLOW_SUB_FLOAT64 = 0xAB,

    /*
    GLOW_SHIFT_LEFT = 0xAC,
    GLOW_SHIFT_RIGHT = 0xAD,
    GLOW_SHIFT_RIGHT_ARITHMETIC = 0xAE,
    */

    /*!
     * \brief allocates an object and stores the result on the stack
     *
     * This command allocates a new object. The type of the object to allocate
     * is stored in the next four bytes as an argument.
     */
    GLOW_ALLOCATE_OBJECT = 0xB0,

    /*!
     * \brief loads an element from an array
     *
     * dereferences an array in the operand stack and an index on top of it
     * (index on top of reference, both word size).
     * This command is followed by a 32-bit unsigned integer defining the
     * size of the structure in the array.
     */
    GLOW_DEREF_ARRAY_INT8 = 0xE0,
    GLOW_DEREF_ARRAY_INT16 = 0xE1,
    GLOW_DEREF_ARRAY_INT32 = 0xE2,
    GLOW_DEREF_ARRAY_INT64 = 0xE3,
    GLOW_DEREF_ARRAY_REFERENCE = 0xE4,

    /*!
     * \brief loads an element from an array
     *
     * dereferences a pointer in the operand stack and adds an immediate
     * offset to it (the offset is the 32-bit operand).
     */
    GLOW_DEREF_INT8 = 0xE5,
    GLOW_DEREF_INT16 = 0xE6,
    GLOW_DEREF_INT32 = 0xE7,
    GLOW_DEREF_INT64 = 0xE8,
    GLOW_DEREF_REFERENCE = 0xE9,

    GLOW_STORE_IN_FIELD_8 = 0xEA,

    /*!
     * calls a native function whose index is stored in the next 4 bytes
     */
    GLOW_CALL_NATIVE = 0x07,

    /*!
     * terminates the execution of the program
     */
    GLOW_EXIT = 0xEF,
} glow_instruction;


#endif // GLOW_BYTECODE_H_
