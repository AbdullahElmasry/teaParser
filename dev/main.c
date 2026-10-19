#include <stdio.h>


typedef struct 
{
    char *pattern;
    char *filename;
    int context;
    _Bool line_numbers;
} args_t;

args_t make_default_args(){
    args_t args;
    args.context = 1;
    args.line_numbers = 0;

    return args;
}

void print_help(char *name){
    printf(">> bad usage of: %s \n", name);
}


// parse arguments and write to *args
int parse_args(int argc, char **argv, args_t *args){
    if(argc < 3){
        // missing arguments
        return 0;
    }

    args->pattern = argv[1];
    args->filename = argv[2];

    for(int i=3; i < argc; i++){
        if(!strcmp(argv[i], "-C")){
            args->context = atoi(argv[++i]);
        }
        else if(!strcmp(argv[i], "-n")){
            args->line_numbers = 1;
        }
    }

    return 1;
}


int main(int argc, char **argv){

    args_t args = make_default_args();
    if (!parse_args(argc, argv, &args)){
        print_help(argv[0]);
        return 1;
    }


    printf("%s\n", args.pattern);
    printf("%s\n", args.filename);
    printf("%d\n", args.context);
    // printf("%s\n", args.line_numbers);

    return 0;
}