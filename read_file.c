#include "read_file.h"

status find_comment_end(char ** string)
{
    int i = 0;
    while ((*string)[i])
    {
        if ((*string)[i] == ']')
        {
            shift_string(string, i + 1);
            return success;
        }
    }
    if ((*string)[0] == ' ') free_from_delims(&string);
    return fail;
}

status read_file_with_settings(FILE *file, Current_settings_ptr settings)
{
    if(!file) return invalid_function_argument;

    char *line = NULL;
    size_t size = 0;
    int flag = 0;
    status error;
    int command_flag = 0;
    while(getline(&line, &size, file) > 0)
    {
        char * token;
        if ((error = my_strtok(&token, &line, " \n\t")))
        {
            free(line);
            return error;
        }
        while (token)
        {
            if (flag && find_comment_end(&token) == success) flag = 0;
            else if (flag) break;
            if (token[0] == '#') break;
            else if (token[0] == '[') flag = 1;

            size_t token_size = strlen(token);

            if (command_flag && !flag)
            {
                free(token);
                free(line);
                return invalid_settings_file;
            }

            if (!strcmp(token, "right=") && !flag)
            {
                settings->operation_result_type = right;
                command_flag = 1;
            }
            else if (!strcmp(token, "left=") && !flag)
            {
                settings->operation_result_type = left;
                command_flag = 1;
            }
            else if (token[0] == '(' && token[1] == ')' && !flag)
            {
                operation operation_name;
                if (is_operation(settings, token + 2, &operation_name) == success) change_basic_syntax(settings, operation_name, right);
                else
                {
                    free(token);
                    free(line);
                    return invalid_settings_file;
                }
                command_flag = 1;
            }
            else if (token[size - 1] == ')' && size - 2 >= 0 && token[size - 2] == '(' && !flag)
            {
                operation operation_name;
                token[size - 1] = token[size - 2] = 0;
                if (is_operation(settings, token, &operation_name) == success) change_basic_syntax(settings, operation_name, left);
                else
                {
                    free(token);
                    free(line);
                    return invalid_settings_file;
                }
                command_flag = 1;
            }
            else if (token[0] == '(' && size - 2 >= 0 && token[size - 1] == ')' && !flag)
            {
                operation operation_name;
                token[size - 1] = 0;
                if (is_operation(settings, token + 1, &operation_name) == success && settings->basic_types[operation_name] == binary) change_basic_syntax(settings, operation_name, middle);
                else
                {
                    free(token);
                    free(line);
                    return invalid_settings_file;
                }
                command_flag = 1;
            }
            free(token);
            if ((error = my_strtok(&token, &line, " \n\t")))
            {
                free(line);
                return error;
            }
        }
        free(token);
        free(line);
    }
    return success;
}