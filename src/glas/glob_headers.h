#ifndef GLOW_GLOB_HEADERS_H_
#define GLOW_GLOB_HEADERS_H_

#include "../integers.h"

/*!
 *
 */
typedef struct
{
    /*! "GLOB" characters */
    glow_uint8      header[4];

    /*! version of the file */
    glow_uint16     version[4];

    glow_uint32     symbol_table_offset;
    glow_uint32     symbol_table_length;

    glow_uint32     constant_table_offset;
    glow_uint32     constant_table_length;

    /*!
     * offset (from beginning of the file) to the bytecode data
     * (the value must be stored in little-endian mode)
     */
    glow_uint32     bytecode_offset;
    glow_uint32     bytecode_length;

} glow_glob_file;


/*!
 *
 */
enum {
    GLOB_CLASS_REFERENCE,
    GLOB_METHOD_REFERENCE,
};

/*!
 *
 */
typedef struct
{
    glow_uint32     type;
    glow_uint32     argument;
} glow_glob_symbol_table_entry;

#endif // GLOW_GLOB_HEADERS_H_
