#include "glow_error.h"
#include <string.h>

char error_buffer[2048];

const char* glow_get_last_error(void)
{
    return error_buffer;
}


void glow_set_last_error(const char* new_message)
{
    size_t len = strlen(new_message);

    if (len >= sizeof error_buffer)
        len = sizeof error_buffer - 1;

    memcpy(error_buffer, new_message, len);
}


