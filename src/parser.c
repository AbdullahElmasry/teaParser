#include <stdlib.h>
#include "internal.h"


tp_parser *tp_parser_create(void){
    tp_parser *parser = calloc(1, sizeof(*parser));  // calloc instead of malloc because calloc is allocating and initializing with zeros

    if(parser == NULL){        // memory allocation can fail, so this helps debugging
        return NULL;
    }

    return parser;
}

void tp_parser_destroy(tp_parser *parser){
    free(parser);
}