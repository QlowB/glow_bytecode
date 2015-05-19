#ifndef GLOW_GLOB_HEADERS_H_
#define GLOW_GLOB_HEADERS_H_

#include "../integers.h"

/*!
 * The glob file format consists of a header (type: glow_glob_header)
 * and the byte code data. The bytecode data is stored at an absolute
 * offset in the file, specified by bytecode_offset in glow_glob_header.
 */
typedef struct
{
    /*! "GLOB" characters */
    glow_uint8      glob[4];

    /*! version of the file */
    glow_uint16     version[4];

    /*!
     * offset (from beginning of the file) to the bytecode data
     * (the value must be stored in little-endian mode)
     */
    glow_uint32     bytecode_offset;

    /*!
     * offset to a table which contains
     */
    glow_uint32     function_table_offset;

    /*!
     * offset to the table with function entries
     */
    glow_uint32     function_calls_offset;
} glow_glob_header;


typedef struct
{
} glow_glob_function_table_entry;

#endif // GLOW_GLOB_HEADERS_H_
