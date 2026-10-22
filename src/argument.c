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


static char *copy_string(const char *s){

}

static int has_conflict(){}

static tp_status ensure_capacity(tp_parser *parser) {}

static tp_status register_arguments(){}

tp_status tp_add_flag(){}

tp_status tp_add_option(){}

tp_status tp_add_positional(){}

