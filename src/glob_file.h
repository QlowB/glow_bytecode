#ifndef GLOW_GLOB_HEADERS_H_
#define GLOW_GLOB_HEADERS_H_

#include "../integers.h"
#include <stdio.h>


/*!
 *
 */
enum glow_glob_symbol_type {
    GLOB_CLASS_REFERENCE = 0x01,
    GLOB_METHOD_REFERENCE = 0x02,
    GLOB_CLASS_DEFINITION = 0x03,
    GLOB_METHOD_DEFINITION = 0x04,
};


/*!
 *
 */
typedef struct
{
    glow_uint8      type;
    glow_uint32     argument1;
    glow_uint32     argument2;
} glow_symbol_table_entry;


/*!
 *
 */
enum glow_glob_constant_type {
    GLOB_CONSTANT_STRING = 0x01,
};


/*!
 *
 */
typedef struct
{
    glow_uint8      type;
    glow_uint32     length;
    char*           data;
} glow_constant_table_entry;


/*!
 *
 */
typedef struct
{
    glow_uint16                 version;

    size_t                      symbol_table_entry_count;
    glow_symbol_table_entry*    symbols;

    size_t                      constant_table_entry_count;
    glow_constant_table_entry*  constants;

    size_t                      bytecode_size;
    char*                       bytecode_data;
} glow_object_content;


/*!
 * \brief load the content of a Glow object file
 * \param content the data structure to store the content to
 * \param file stream where the Glow object file is read from
 * \return 0 on success, nonzero otherwise
 */
int glow_load_glob(glow_object_content* content, FILE* file);


/*!
 * \brief store the content of a Glow object content into a file
 * \param content the data structure to store
 * \param file stream where the Glow object file is written to
 * \return 0 on success, nonzero otherwise
 */
int glow_save_glob(glow_object_content* content, FILE* file);


/*!
 * \brief frees the data allocated in \link glow_load_glob
 * \param content the data structure containing the allocated data
 */
void glow_free_object_content(glow_object_content* content);

#endif // GLOW_GLOB_HEADERS_H_

