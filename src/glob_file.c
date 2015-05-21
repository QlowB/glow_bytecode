#include "glob_file.h"
#include "glow_error.h"

static const glow_uint16 supported_version[4] = {0x0000, 0x0000, 0x0000, 0x0001};

/*!
 * local data structure representing the Glow object file header
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

} glob_header;



int glow_load_glob(glow_object_content* content, FILE* file)
{
    glob_header head;

    memset(content, 0, sizeof *content);

    fread(&head.header, 1, 4, file);
    fread(&head.version, 2, 4, file);
    fread(&head.symbol_table_offset, 4, 1, file);
    fread(&head.symbol_table_length, 4, 1, file);
    fread(&head.constant_table_offset, 4, 1, file);
    fread(&head.constant_table_length, 4, 1, file);
    fread(&head.bytecode_offset, 4, 1, file);
    fread(&head.bytecode_length, 4, 1, file);

    if (memcmp(supported_version, head.version) != 0) {
        glow_set_last_error("incopatible glow object version");
        return 1;
    }

    content->symbol_table_entry_count = head.symbol_table_length;
    content->constant_table_entry_count = head.constant_table_length;

    content->symbols = malloc(head.symbol_table_length * sizeof (glow_symbol_table_entry));
    content->constants = malloc(head.constant_table_length * sizeof (glow_constant_table_entry));

    fseek(file, head.symbol_table_offset, SEEK_SET);
    for (size_t i = 0; i < head.symbol_table_length; i++) {
        fread(&content->symbols[i].type, 1, 1, file);
        fread(&content->symbols[i].argument, 4, 1, file);
    }

    fseek(file, head.constant_table_offset, SEEK_SET);
    for (size_t i = 0; i < head.constant_table_length; i++) {
        fread(&content->constants[i].type, 1, 1, file);
        fread(&content->constants[i].length, 4, 1, file);

        content->constants[i].data = malloc(content->constants[i].length);
        fread(&content->constants[i].data, 1, content->constants[i].length, file);
    }

    fseek(file, head.bytecode_offset, SEEK_SET);
    content->bytecode_data = malloc(head.bytecode_length);
    fread(content->bytecode_data, 1, head.bytecode_length, file);

    return 0;
}


void glow_free_object_content(glow_object_content* content)
{
    if (content == 0)
        return;

    if (content->symbols != 0)
        free(content->symbols);
}



