#ifndef TEAPARSER_H
#define TEAPARSER_H

#ifdef __cplusplus      // for c++ support
extern "C" {
#endif

typedef struct tp_parser tp_parser;

tp_parser *tp_parser_create(void);

void tp_parser_destroy(tp_parser *parser);


#ifdef __cplusplus
}
#endif



#endif