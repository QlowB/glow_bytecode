#include "run_options.h"

#include <string.h>
#include <stdlib.h>

struct option_and_link
{
    const char* option;
    struct option_and_link* next;
};

void glow_read_options(glow_run_options* opt, int argc, const char** argv)
{
    struct option_and_link* first = 0;
    struct option_and_link* current = 0;
    int option_count = 0;
    opt->input_file = 0;
    opt->output_file = 0;
    for (int i = 1; i < argc; i++) {
        const char* arg = argv[i];
        
        if (strcmp(arg, "-o") == 0) {
            if (argc > i + 1) {
                opt->output_file = argv[++i];
            }
        }
        else if (arg[0] == '-') {
            option_count++;
            if (current == 0) {
                first = current = malloc(sizeof(struct option_and_link));
            } else {
                current->next = malloc(sizeof(struct option_and_link));
                current = current->next;    
            }
            current->next = 0;
            current->option = arg;
        }
        else {
            opt->input_file = arg;
        }
    }
    opt->options = malloc((option_count + 1) * sizeof(const char*));
    for (int i = 0; i < option_count; i++) {
        opt->options[i] = first->option;
        first = first->next;
    }
    opt->options[option_count] = 0;
}


int glow_has_option(glow_run_options* opt, const char* option)
{
    int index = 0;
    while (opt->options[index]) {
        if (strcmp(opt->options[index], option) == 0) {
            return 1;
        }
    }
    return 0;
}










