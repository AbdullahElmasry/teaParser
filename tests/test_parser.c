
#include <assert.h>
#include <teaparser.h>
#include <stddef.h>

int main(void)
{
    tp_parser *parser = tp_parser_create();

    assert(parser != NULL);

    tp_parser_destroy(parser);

    /* Destroying NULL is safe. */
    tp_parser_destroy(NULL);

    return 0;
}
