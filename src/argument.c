#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "internal.h"

// Private helpers and validations

static int valid_identifier(const char *s){
    if(s == NULL || s[0] == '\0'){  // checking the pointer and the value 
        return 0;
    }

    char c = s[0];

    // ensuring english letters upper or lower or '_'
    if (!((c >= 'a'  && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')){
        return 0; // zero for false 
    }

    for (size_t i=1; s[i] != '\0'; ++i){
        c = s[i];

        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '-')){
            return 0;
        }
    }

    return 1;
}


static int valid_short_name(const char *s){
    if(s == NULL || s[0] == '\0'){  // checking the pointer and the value 
        return 0;
    }

    char c = s[0];

    if(strlen(s) != 2){
        return 0;
    }

    // ensuring first character is hyphen '-'
    if (c != '-'){   // if '-' returns if(false) so no return zero 

        return 0;
    }

    if(!(s[1] >= 'a' && s[1] <= 'z' )){
        return 0;
    }

    return 1;
}


/*
 * Command-line option identifier validation rules
 * ------------------------------------------------
 *
 * Input          Valid?   Reason
 * ------------   ------   -----------------------------------------
 * --verbose      Yes      Correct format
 * --output-file  Yes      Hyphens are allowed inside
 * --port8080     Yes      Digits allowed after first letter
 * --_debug       Yes      Underscore allowed
 * --123port      No       Identifier cannot start with digit
 * --             No       Missing identifier
 * -verbose       No       Only one hyphen
 * verbose        No       Missing prefix
 * NULL           No       Invalid pointer
 */
static int valid_long_name(const char *s){

    char c;

    if(s == NULL || s[0] == '\0'){  // checking the pointer and the value 
        return 0;
    }

    // ensure first two charachters are "--"
    if ((s[0] != '-') || (s[1] != '-')){      // -verbose
        return 0;
    }

    if(((s[2] >= '0' && s[2] <= '9') || (s[2] == '\0') || (s[2] == '-'))){
        return 0;
    }

    for (size_t i = 2; s[i] != '\0'; ++i){
        c = s[i];

        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '-') || (c == '_') || (c >= '0' && c <= '9') )){
            return 0;
        }
    }


    return 1;
}


/*
also this function could be like this 
static int valid_long_name(const char *s)
{
    if (s == NULL)
        return 0;

    if (s[0] != '-' || s[1] != '-')
        return 0;

    return valid_identifier(s + 2);
}
*/

//============================================================
/*
 * str_dup - duplicate a null-terminated string on the heap
 *
 * Contract:
 *   1. Accepts a valid, null-terminated C string.
 *   2. Allocates enough memory to hold the string, including the '\0'.
 *   3. Copies the contents into the new allocation.
 *   4. Returns a pointer to the new allocation.
 *   5. Returns NULL if allocation fails.
 *
 * NULL input is also allowed and returns NULL. Callers that treat the
 * argument as optional (e.g. a missing alias) must therefore distinguish
 * "no alias supplied" from "copy failed" by checking the input themselves,
 * not by inspecting the return value alone.
 */
static char *copy_string(const char *s){

    /* SIZE_MAX is a standard macro in C, not a variable, and it is defined in the <stdint.h> header since C99
       it expands to the maximum value that a size_t object can hold, and its exact value depends on the platform (for example, 18446744073709551615 on a typical 64-bit system)
    */

    if(s == NULL){
        return NULL;
    }

    size_t length = strlen(s);

    if(length == SIZE_MAX){
        return NULL;
    }

    char *copy = malloc(length +1);

    // verify allocation succeeded
    if(copy == NULL){
        return NULL;
    }

    memcpy(copy, s, length+1);

    return copy;        // a pointer to allocated memory
}

// passing the parser containing registered arguments along side with the new argument name short name and long name to compare
static int has_conflict(const tp_parser *parser, const char *name, const char* short_name, const char* long_name){
    for(size_t i=0; i < parser->argument_count; ++i ){

        // parser is the struct containing the argument data such as arguments, argument_count, argument_capacity
        const tp_argument *arg = &parser->arguments[i];
        // now arg containing the argument struct data such as name, short_name, long_name

        // check internal name
        if(strcmp(arg->name , name)){
            return 0;    // conflict found
        }

        // check short name
        if(short_name != NULL && 
           arg->short_name != NULL &&
           (strcmp(arg->short_name, short_name) == 0 )){

            return 0;     // conflict found
        }

        // check long name alias
        if(long_name != NULL && 
           arg->long_name != NULL &&
           (strcmp(arg->long_name, long_name) == 0 )){
            return 0;     // conflict found
        }

        return 1;       // no conflict return true
    }
}

// verify there is enough allocated space for the next argument
static tp_status ensure_capacity(tp_parser *parser) {

    /*
        get the current size,
        which is the argument_count from tp_parser struct
    */
    const size_t current_argument_count = parser->argument_count;
    const size_t current_argument_capacity = parser->argument_capacity; 

    if(current_argument_count < current_argument_capacity){
        // do nothing
        return TP_OK;
    }

    /*
        * Maximum number of tp_argument elements
        * without overflowing size_t.
     */
    const size_t max_elements = SIZE_MAX / sizeof(tp_argument);

    /* Cannot allocate space for another argument. */
    if (current_argument_count >= max_elements) {
        return TP_ERROR_OUT_OF_MEMORY;
    }

    size_t new_capacity;
    
    if (current_argument_capacity == 0) {
        new_capacity = max_elements < 4 ? max_elements : 4;
    }
    
    /* Prevent overflow when doubling capacity. */
    else if (current_argument_capacity > max_elements / 2) {
        new_capacity = max_elements;
    }

    /* Normal geometric growth */
    else {
        new_capacity = current_argument_capacity * 2;
    }

    tp_argument *new_arguments = realloc(parser->arguments, new_capacity * sizeof(tp_argument));

    if (new_arguments == NULL) {
        return TP_ERROR_OUT_OF_MEMORY;
    }

    parser->arguments = new_arguments;
    parser->argument_capacity = new_capacity;

    return TP_OK;
}


static tp_status register_arguments(
    tp_parser *parser,
    const char *name,
    const char *short_name,
    const char *long_name,
    tp_arg_kind kind
)
{
    // Validate the identifier 
    if (parser == NULL || !valid_identifier(name)) {
        return TP_ERROR_INVALID_ARGUMENT;
    }

    if (kind == TP_ARG_POSITIONAL) {

        /* Positional arguments cannot have aliases */
        if (short_name != NULL || long_name != NULL) {
            return TP_ERROR_INVALID_ARGUMENT;
        }
    }

    // Flags and options require at least one alias 
    else if (kind == TP_ARG_FLAG || kind == TP_ARG_OPTION) {

        if (short_name == NULL && long_name == NULL) {
            return TP_ERROR_INVALID_ARGUMENT;
        }

        if (short_name != NULL && !valid_short_name(short_name)) {
            return TP_ERROR_INVALID_ARGUMENT;
        }

        if (long_name != NULL && !valid_long_name(long_name)) {
            return TP_ERROR_INVALID_ARGUMENT;
        }
    }

    else {
        return TP_ERROR_INVALID_ARGUMENT;
    }

    // Check for duplicate names or aliases 
    if (!has_conflict(parser, name, short_name, long_name)) {
        return TP_ERROR_DUPLICATE_ARGUMENT;
    }

    // Ensure sufficient capacity 
    tp_status status = ensure_capacity(parser);

    if (status != TP_OK) {
        return status;
    }

    /* Get the next available argument slot */
    tp_argument *arg = &parser->arguments[parser->argument_count];

    /* Initialize the argument */
    *arg = (tp_argument){0};

    arg->kind = kind;

    // Copy argument names */
    arg->name = copy_string(name);
    arg->short_name = copy_string(short_name);
    arg->long_name = copy_string(long_name);

    // Check for allocation failures */
    if (arg->name == NULL ||
        (short_name != NULL && arg->short_name == NULL) ||
        (long_name != NULL && arg->long_name == NULL)) {

        /* Free successfully allocated strings */
        free(arg->name);
        free(arg->short_name);
        free(arg->long_name);

        /* Reset the unused argument slot */
        *arg = (tp_argument){0};

        return TP_ERROR_OUT_OF_MEMORY;
    }

    // commit the registered argument 
    parser->argument_count++;

    return TP_OK;
}


tp_status tp_add_flag(){}

tp_status tp_add_option(){}

tp_status tp_add_positional(){}

