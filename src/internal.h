#ifndef SRC_INTERNAL_H
#define SRC_INTERNAL_H

#include <stddef.h>
#include "teaparser.h"

struct tp_parser {
    size_t argument_count;
    size_t argument_capacity;
};

#endif