#ifndef TEAPARSER_H
#define TEAPARSER_H

#ifdef __cplusplus      // for c++ support
extern "C" {
#endif

typedef struct tp_parser tp_parser;


typedef enum{
    TP_ARG_FLAG,
    TP_ARG_OPTION,
    TP_ARG_POSITIONAL
} tp_arg_kind;


typedef enum {
    TP_OK = 0,
    TP_ERROR_INVALID_ARGUMENT,
    TP_ERROR_DUPLICATE_ARGUMENT,
    TP_ERROR_OUT_OF_MEMORY
} tp_status;


// function declaration of type tp_status, that registers new flag 
tp_status tp_add_flag(
    tp_parser *parser,
    const char *name,
    const char *short_name,
    const char *long_name
);

// register new option
tp_status tp_add_option(
    tp_parser *parser,
    const char *name,
    const char *short_name,
    const char *long_name
);

// register new positional
tp_status tp_add_positional(
    tp_parser *parser,
    const char *name
);

// parser lifecycle 
tp_parser *tp_parser_create(void);
void tp_parser_destroy(tp_parser *parser);





#ifdef __cplusplus
}
#endif



#endif