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
     * pushes an eight-byte constant (following this instruction)
     * on the execution stack
     *
     * \note 64-bit floating point numbers can also be considered eight-byte
     * constants
     */
    GLOW_LOAD_CONSTANT_WORD = 0x01,

    /*!
     * pushes a eight-byte local variable (stack offset in 4 bytes following
     * this instruction) on the execution stack
     */
    GLOW_LOAD_LOCAL_WORD = 0x02,

    /*!
     * pushes a pointer-sized local variable (stack offset in 4 bytes following
     * this instruction) onto the execution stack
     */
    GLOW_LOAD_LOCAL_REFERENCE = 0x03,

    /*!
     * saves the topmost one-byte operand variable in the call stack
     * (stack offset in 4 bytes following this instruction)
     */

    GLOW_STORE_LOCAL_WORD = 0x04,

    /*!
     * pushes a word-size local variable (stack offset in 4 bytes following
     * this instruction) on the execution stack
     */
    GLOW_STORE_LOCAL_REFERENCE = 0x05,

    GLOW_JUMP = 0x08,

    /*!
     * pops the topmost entry from the operand stack and deletes it.
     */
    GLOW_POP = 0x0F,

    /*!
     * pops two 8-bit integers from the operand stack and pushes the comparison
     * result as an integer:
     * 1, if the integer on top was less than the other, -1 if it was the other
     * way around and 0 if they are equal.
     */
    GLOW_COMPARE_INT8 = 0x10,
    GLOW_COMPARE_INT16 = 0x11,
    GLOW_COMPARE_INT32 = 0x12,
    GLOW_COMPARE_INT64 = 0x13,

    GLOW_COMPARE_UINT = 0x14,
    
    GLOW_COMPARE_FLOAT32 = 0x15,
    GLOW_COMPARE_FLOAT64 = 0x16,

    /*!
     * \brief jumps to a relative location stored in the next four bytes.
     *
     * pops one word from the operand stack; finishes execution of this
     * instruction, then adds the 32-bit relative jump address to the
     * instruction pointer, only if the popped word equals zero
     */
    GLOW_JUMP_IF_ZERO = 0x20,
    GLOW_JUMP_IF_EQUAL = 0x20,
    
    /*!
     * \brief jumps to a relative location stored in the next four bytes.
     *
     * pops one word from the operand stack; finishes execution of this
     * instruction, then adds the 32-bit relative jump address to the
     * instruction pointer, only if the popped word doesn't equal zero
     */ 
    GLOW_JUMP_IF_NOT_EQUAL = 0x21,
    GLOW_JUMP_IF_NOT_ZERO = 0x21,

    GLOW_JUMP_IF_GREATER = 0x22,
    GLOW_JUMP_IF_LESS = 0x23,
    GLOW_JUMP_IF_GREATER_OR_EQUAL = 0x24,
    GLOW_JUMP_IF_LESS_OR_EQUAL = 0x25,

    
    /*!
     * pops and sign extends a 8-bit integer to a 16-bit integer and pushes
     * it again
     */
    GLOW_SIGN_EXTEND_INT8_TO_INT16 = 0x30,
    
    /*!
     * pops and sign extends a 8-bit integer to a 32-bit integer and pushes
     * it again
     */
    GLOW_SIGN_EXTEND_INT8_TO_INT32 = 0x31,
    
    /*!
     * pops and sign extends a 8-bit integer to a 64-bit integer and pushes
     * it again
     */
    GLOW_SIGN_EXTEND_INT8_TO_INT64 = 0x32,
    
    /*!
     * pops and sign extends a 16-bit integer to a 32-bit integer and pushes
     * it again
     */
    GLOW_SIGN_EXTEND_INT16_TO_INT32 = 0x33,
    
    /*!
     * pops and sign extends a 16-bit integer to a 64-bit integer and pushes
     * it again
     */
    GLOW_SIGN_EXTEND_INT16_TO_INT64 = 0x34,
    
    /*!
     * pops and sign extends a 32-bit integer to a -bit integer and pushes
     * it again
     */
    GLOW_SIGN_EXTEND_INT32_TO_INT64 = 0x35,
    
    GLOW_NEGATE_INT8 = 0x36,
    GLOW_NEGATE_INT16 = 0x37,
    GLOW_NEGATE_INT32 = 0x38,
    GLOW_NEGATE_INT64 = 0x39,
    
    GLOW_NEGATE_FLOAT32 = 0x3A,
    GLOW_NEGATE_FLOAT64 = 0x3B,

    /*!
     * duplicates the topmost stack entry
     */
    GLOW_DUPLICATE = 0x3C,

    /*!
     * pops two integers from the operands stack, adds them together,
     * and pushes them again on the stack.
     */
    GLOW_ADD_INT = 0x40,

    /*!
     * pops two 32-bit floats from the operands stack, adds them together,
     * and pushes them again on the stack.
     */
    GLOW_ADD_FLOAT32 = 0x44,
    
    /*!
     * pops two 64-bit floats from the operands stack, adds them together,
     * and pushes them again on the stack.
     */
    GLOW_ADD_FLOAT64 = 0x45,

    GLOW_MULT_INT8 = 0x48,
    GLOW_MULT_INT16 = 0x49,
    GLOW_MULT_INT32 = 0x4A,

    /*!
     * multiplying two 64-bit numbers produces two outputs on the stack
     * the overflow on top and the result in the second place.
     */
    GLOW_MULT_INT64 = 0x4B,

    /*!
     * multiplying two unsigned integers always produces two outputs on
     * the stack, the overflow on top and the result in the second place.
     */
    GLOW_MULT_UINT = 0x4C,

    /*!
     * multiplying floating point numbers produces only one output on the stack
     */
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

    GLOW_AND_INT = 0x62,

    GLOW_OR_INT = 0x66,

    GLOW_XOR_INT = 0x6A,

    /*
     * invert integers
     */
    GLOW_NOT_INT = 0x6E,
    GLOW_NOT_BOOL = 0x6F,

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
    GLOW_SUB_UINT = 0xAA,
    GLOW_SUB_FLOAT32 = 0xAB,
    GLOW_SUB_FLOAT64 = 0xAC,

    /*!
     * \brief allocates an object and stores the created reference on the stack
     *
     * This command allocates a new object. The type of the object to allocate
     * is stored in the next four bytes as an argument.
     */
    GLOW_ALLOCATE_OBJECT = 0xB0,
    GLOW_DELETE_OBJECT = 0xB1,
    
    
    /*!
     * \brief call a static function
     *
     * The following 4 bytes contain an entry to the method in the symbol table.
     * The first argument of the entry should link to the the class name, while the second
     * points to the the method name.
     *
     * Arguments are passed in the operand stack.
     */
    GLOW_CALL_STATIC = 0xB2,
    
    /*!
     * \brief call a member function
     *
     * The following 4 bytes contain an entry to the method in the symbol table.
     * The first argument of the entry should link to the the class name, while the second
     * points to the the method name.
     *
     * Arguments are passed in the operand stack.
     */
    GLOW_CALL_MEMBER = 0xB3,
    
    /*!
     * \brief call a virtual member function
     * 
     * The following 4 bytes contain an entry to the method in the symbol table.
     * The first argument of the entry should link to the the class name, while the second
     * points to the the method name.
     *
     * Arguments are passed in the operand stack.
     */
    GLOW_CALL_MEMBER_VIRTUAL = 0xB4,


    GLOW_LOAD_FROM_FIELD_8 = 0xD6,
    GLOW_LOAD_FROM_FIELD_16 = 0xD7,
    GLOW_LOAD_FROM_FIELD_32 = 0xD8,
    GLOW_LOAD_FROM_FIELD_64 = 0xD9,
    GLOW_LOAD_FROM_FIELD_REFERENCE = 0xDA,

    /*!
     * stores an 8-bit value from the top of the stack into a field
     * of an object reference (second position on the stack). The field
     * is identified by four bytes following this instruction.
     */
    GLOW_STORE_IN_FIELD_8 = 0xDB,
    GLOW_STORE_IN_FIELD_16 = 0xDC,
    GLOW_STORE_IN_FIELD_32 = 0xDD,
    GLOW_STORE_IN_FIELD_64 = 0xDE,
    GLOW_STORE_IN_FIELD_REFERENCE = 0xDF,
    
    /*!
     * \brief loads an element from an array
     *
     * dereferences an array in the operand stack and an index on top of it
     * (index on top of reference, 64-bits).
     * This command is followed by a 32-bit unsigned integer defining the
     * size of the structure in the array.
     */
    GLOW_LOAD_FROM_ARRAY_8 = 0xE0,
    GLOW_LOAD_FROM_ARRAY_16 = 0xE1,
    GLOW_LOAD_FROM_ARRAY_32 = 0xE2,
    GLOW_LOAD_FROM_ARRAY_64 = 0xE3,
    GLOW_LOAD_FROM_ARRAY_REFERENCE = 0xE4,
    
    /*!
     * \brief stores an element in an array
     *
     * stores the value on top of the stack in the array at an index.
     *
     * stack before execution should be:
     *      value
     *      index
     *      array_reference
     */
    GLOW_STORE_IN_ARRAY_8 = 0xE5,
    GLOW_STORE_IN_ARRAY_16 = 0xE6,
    GLOW_STORE_IN_ARRAY_32 = 0xE7,
    GLOW_STORE_IN_ARRAY_64 = 0xE8,
    GLOW_STORE_IN_ARRAY_REFERENCE = 0xE9,
    
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

