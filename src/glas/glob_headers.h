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

    /*!
     * offset (from beginning of the file) to the bytecode data
     * (the value must be stored in little-endian mode)
     */
    glow_uint32     bytecode_offset;

    /*!
     * offset to a table which contains
     */
    glow_uint32     linker_table_offset;
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
    glow_uint8      type;
    glow_uint32     length;
    char            entry[0];
} glow_glob_linker_table_entry;

#endif // GLOW_GLOB_HEADERS_H_
