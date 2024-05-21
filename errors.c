#include "errors.h"

void print_error(status const error)
{
    const char * main_error = "[ERROR]";
    const char * errors[] = 
    {
        "Incorrect command line arguments.",
        "Memory error.",
        "File open error.",
        "Invalid function argument.",
        "Invalid base.",
        "Invalid variable.",
        "Variable is not in base.",
        "Variable is not initialized.",
        "Invalid lexeme."
    };
    switch (error)
    {
        case command_line_arguments_error:
            printf("%s %s\n", main_error, errors[0]);
            break;
        case no_memory:
            printf("%s %s\n", main_error, errors[1]);
            break;
        case file_open_error:
            printf("%s %s\n", main_error, errors[2]);
            break;
        case invalid_function_argument:
            printf("%s %s\n", main_error, errors[3]);
            break;
        case invalid_base:
            printf("%s %s\n", main_error, errors[4]);
            break;
        case invalid_variable:
            printf("%s %s\n", main_error, errors[5]);
            break;
        case not_in_base:
            printf("%s %s\n", main_error, errors[6]);
            break;
        case cannot_find:
            printf("%s %s\n", main_error, errors[7]);
            break;
        case invalid_lexeme:
            printf("%s %s\n", main_error, errors[8]);
            break;

        default: break;
    }
}