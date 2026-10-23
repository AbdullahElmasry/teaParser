#ifndef SRC_INTERNAL_H
#define SRC_INTERNAL_H

#include <stddef.h>
#include "teaparser.h"

typedef struct {
    char *name;
    char *short_name;
    char *long_name;
    tp_arg_kind kind;
}tp_argument;



struct tp_parser {
    tp_argument *argments;
    
    size_t argument_count;
    size_t argument_capacity;
};

#endif