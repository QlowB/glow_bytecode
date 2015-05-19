#ifndef GLOW_RUN_OPTIONS_H_
#define GLOW_RUN_OPTIONS_H_


typedef struct glow_run_opt_
{
    const char* input_file;
    const char* output_file;
    const char** options;
} glow_run_options;


void glow_read_options(glow_run_options*, int argc, const char** argv);


int glow_has_option(glow_run_options*, const char* option);



#endif // GLOW_RUN_OPTIONS_H_

