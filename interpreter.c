#include "interpreter.h"


status add_to_expression(char ** expression, char * to_add)
{
    size_t old_size = strlen(*expression);
    size_t new_size = old_size + strlen(to_add);
    char * tmp = realloc(*expression, (new_size + 2) * sizeof(char));
    if (!tmp)
    {
        free(expression);
        return no_memory;
    }
    *expression = tmp;
    if (old_size != 0)
    {
        (*expression)[old_size] = ',';
        old_size++;
    }
    strcpy(*expression + old_size, to_add);
    return success;
}

status make_unary_expression(Trie_ptr trie, operation operation_name, uint32_t value, uint32_t * result, int input_base, int output_base, int assign_base)
{
    switch (operation_name)
    {
        case NOT:
            return not_operation(trie, value, result);
        case INPUT:
            return input_operation(trie, input_base, result);
        case OUTPUT:
            return output_operation(trie, value, output_base);
        default:
            break;
    }
    return fail;
}

status make_binary_expression(Trie_ptr trie, operation operation_name, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    switch (operation_name)
    {
        case ADD:
            return add_operation(trie, value_1, value_2, result);
        case MULT:
            return mult_operation(trie, value_1, value_2, result);
        case SUB:
            return sub_operation(trie, value_1, value_2, result);
        case POW:
            return pow_operation(trie, value_1, value_2, result);
        case DIV:
            return div_operation(trie, value_1, value_2, result);
        case REM:
            return rem_operation(trie, value_1, value_2, result);
        case XOR:
            return xor_operation(trie, value_1, value_2, result);
        case AND:
            return and_operation(trie, value_1, value_2, result);
        case OR:
            return or_operation(trie, value_1, value_2, result);
        default:
            break;
    }
    return fail;
}

status is_delim(char character, const char * delim)
{
    int i = 0;
    while (delim[i])
    {
        if (delim[i] == character) return success;
        ++i;
    }
    return fail;
}

void shift_string(char ** string, int index)
{
    int i = 0;
    int start_index = index;
    while ((*string)[index])
    {
        (*string)[i] = (*string)[index];
        ++i;
        ++index;
    }
    for (int j = i; j < index; ++j) (*string)[j] = 0;
}

status my_strtok(char ** result, char ** st_string, const char * delim)
{
    int is_empty_string = 1;
    char * string = *st_string;
    *result = NULL;
    if (!string[0]) return success;
    char * new_string = (char*)calloc((strlen(string) + 1), sizeof(char));
    if (!new_string) return no_memory;
    
    int i = 0;

    while (string[i] && is_delim(string[i], delim) != success)
    {
        if (string[i] != ' ' && string[i] != '\n' && string[i] != '\t') is_empty_string = 0;
        new_string[i] = string[i];
        ++i;
    }
    if (i == 0 && is_delim(string[i], delim) == success) 
    {
        shift_string(st_string, i + 1);
        free(new_string);
        return my_strtok(result, st_string, delim);
    }
    if (is_empty_string)
    {
        free(new_string);
        return success;
    }
    if (strcmp(delim, ";") == 0 && is_delim(string[i], delim) != success)
    {
        free(new_string);
        return invalid_lexeme;
    }
    if (is_delim(string[i], delim) == success) ++i;

    shift_string(st_string, i);
    free_from_delims(&new_string);
    *result = new_string;
    return success;
}

// тк "=" - тоже функция, пользователь может заменить то, как она выглядит, на многосимвольное выражение. поэтому для него сделаем отдельный strtok
status my_assign_strtok(char ** result, char ** st_string, const char * delim)
{
    int is_empty_string = 1;
    char * string = *st_string;
    *result = NULL;
    if (!string[0]) return success;
    char * new_string = (char*)calloc((strlen(string) + 1), sizeof(char));
    if (!new_string) return no_memory;
    int i = 0;

    char * ptr = strstr(string, delim);
    if (!ptr)
    {
        shift_string(st_string, strlen(string));
        free_from_delims(&new_string);
        *result = new_string;
        return success;
    }
    while (string[i] != *ptr)
    {
        if (string[i] != ' ' && string[i] != '\n' && string[i] != '\t') is_empty_string = 0;
        new_string[i] = string[i];
        ++i;
    }
    if (string[0] == *ptr)
    {
        shift_string(st_string, i + 1);
        free(new_string);
        return my_assign_strtok(result, st_string, delim);
    }
    if (is_empty_string)
    {
        free(new_string);
        return success;
    }
    shift_string(st_string, i + strlen(delim));
    
    free_from_delims(&new_string);
    *result = new_string;
    return success;
}

status solve_expression(Current_settings_ptr settings, Trie_ptr trie, char * st_expression, uint32_t * result, int input_base, int output_base, int assign_base)
{
    if (!st_expression) return invalid_lexeme;
    char * expression = (char*)calloc(strlen(st_expression) + 1, sizeof(char));
    if (!expression) return no_memory;
    strcpy(expression, st_expression);
    char * token;
    status error = my_strtok(&token, &expression, "(), ");
    if (error != success)
    {
        free(expression);
        free(token);
        return error;
    }
    operation operation_name;
    char * tmp_expression = (char*)calloc(1, sizeof(char));
    if (!tmp_expression)
    {
        free(expression);
        return no_memory;
    }
    int terms = 0;
    while (token && is_operation(token, &operation_name) != success)
    {
        terms++;
        error = add_to_expression(&tmp_expression, token);
        if (error != success)
        {
            free(expression);
            free(token);
            return error;
        }
        free(token);
        token = NULL;
        error = my_strtok(&token, &expression, "(), ");
        if (error != success)
        {
            free(expression);
            free(tmp_expression);
            free(token);
            return error;
        }
    }
    if (!token)
    {
        if (is_variable(tmp_expression) == success || is_number(tmp_expression) == success)
        {
            free(expression);
            error = get_value(trie, tmp_expression, result, assign_base);
            free(tmp_expression);
            return error;
        }
        free(expression);
        free(tmp_expression);
        return invalid_lexeme;
    }
    if (is_operation(token, &operation_name) == success)
    {
        free(token);
        token = NULL;
        switch (settings->basic_types[operation_name])
        {
            case unary:
                uint32_t value;
                char * to_solve;
                switch (settings->basic_syntax[operation_name])
                {
                    case left:
                        error = my_strtok(&to_solve, &expression, "(),");
                        if (error != success)
                        {
                            free(expression);
                            free(tmp_expression);
                            return error;
                        }
                        break;
                    case right:
                        to_solve = tmp_expression;
                        break;
                }
                error = solve_expression(settings, trie, to_solve, &value, input_base, output_base, assign_base);
                if (error != success)
                {
                    free(to_solve);
                    free(tmp_expression);
                    free(expression);
                    free(tmp_expression);
                    return error;
                }
                free(to_solve);
                error = make_unary_expression(trie, operation_name, value, result, input_base, output_base, assign_base);
                if (error != success)
                {
                    free(tmp_expression);
                    free(expression);
                    free(tmp_expression);
                    return error;
                }
                free(expression);
                free(tmp_expression);
                return success;
                break;
            case binary:
                uint32_t value_1;
                uint32_t value_2;
                char * to_solve_1;
                char * to_solve_2;
                switch (settings->basic_syntax[operation_name])
                {
                    case left:
                        if (terms != 0)
                        {
                            free(expression);
                            free(tmp_expression);
                            return invalid_lexeme;
                        }
                        error = my_strtok(&to_solve_1, &expression, "(),");
                        if (error != success)
                        {
                            free(expression);
                            free(tmp_expression);
                            return error;
                        }
                        break;
                    case right:
                        if (terms != 2)
                        {
                            free(expression);
                            free(tmp_expression);
                            return invalid_lexeme;
                        }
                        error = my_strtok(&to_solve_1, &tmp_expression, "(),");
                        if (error != success)
                        {
                            free(expression);
                            free(tmp_expression);
                            return error;
                        }
                        break;
                    case middle:
                        if (terms != 1)
                        {
                            free(expression);
                            free(tmp_expression);
                            return invalid_lexeme;
                        }
                        error = my_strtok(&to_solve_1, &tmp_expression, "(), ");
                        if (error != success)
                        {
                            free(expression);
                            free(tmp_expression);
                            return error;
                        }
                        break;
                }
                if (settings->basic_syntax[operation_name] == middle) error = my_strtok(&to_solve_2, &expression, "(), ");
                else if (settings->basic_syntax[operation_name] == right) error = my_strtok(&to_solve_2, &tmp_expression, "(),");
                else error = my_strtok(&to_solve_2, &expression, "(),");
                if (error != success)
                {
                    free(to_solve_1);
                    free(expression);
                    free(tmp_expression);
                    return error;
                }
                error = solve_expression(settings, trie, to_solve_1, &value_1, input_base, output_base, assign_base);
                if (error != success)
                {
                    free(to_solve_1);
                    free(to_solve_2);
                    free(expression);
                    free(tmp_expression);
                    return error;
                }
                free(to_solve_1);
                error = solve_expression(settings, trie, to_solve_2, &value_2, input_base, output_base, assign_base);
                if (error != success)
                {
                    free(to_solve_2);
                    free(expression);
                    free(tmp_expression);
                    return error;
                }
                free(to_solve_2);
                error = make_binary_expression(trie, operation_name, value_1, value_2, result);
                if (error != success)
                {
                    free(expression);
                    free(tmp_expression);
                    return error;
                }
                free(tmp_expression);
                free(expression);
                return success;
                break;
        }
    }
    free(tmp_expression);
    free(token);
    free(expression);
    return invalid_lexeme;
}

status free_from_delims(char ** string)
{
    if (!*string) return success;
    int i = 0;
    while ((*string)[i] && ((*string)[i] == ' ' || (*string)[i] == '\n' || (*string)[i] == '\t')) ++i;
    shift_string(string, i);
    i = strlen(*string) - 1;
    while (i != 0 && ((*string)[i] == ' ' || (*string)[i] == '\n' || (*string)[i] == '\t'))
    {
        (*string)[i] = 0;
        --i;
    }
    return success;
}

status check_comments(char ** st_string, int * is_breakpoint)
{
    char * string = *st_string;
    int i = 0;
    while (string[i] && string[i] != '#') ++i;
    if (!string[i]) return fail;
    int j = i;
    while (string[j] && string[j] != '\n') ++j;
    if (!string[j]) return success; // комментарий еще продолжается
    // если же нашли конец, находится ли внутри BREAKPOINT или нужно просто удалить комментарий (удаляем, просто заменяя все на пробелы)

    // проверка, лежит ли внутри брейкпоинт
    int no = 0;
    char * breakpoint = "BREAKPOINT";
    if (strlen(breakpoint) != j - (i + 1)) no = 1;
    if (!no)
    {
        for (int h = i + 1; h < j; ++h)
        {
            if (string[h] != breakpoint[h])
            {
                no = 1;
                break;
            }
        }
    }
    if (!no) *is_breakpoint = 1;
    // теперь просто убираем комментарий
    for (int h = i; h < j; ++h) (*st_string)[h] = ' ';
    return fail;
}

status scan_buffer(Current_settings_ptr settings, Trie_ptr trie, char * st_buffer, int input_base, int output_base, int assign_base)
{
    char * buffer = (char*)calloc(strlen(st_buffer) + 1, sizeof(char));
    if (!buffer) return no_memory;
    strcpy(buffer, st_buffer);
    status error;
    char * line;
    error = my_strtok(&line, &buffer, ";");
    if (error != success) return error;
    int comment = 0;
    while (line)
    {
        if (comment)
        {
            comment = 0;
            int i = 0;
            while (line[i] && line[i] != '\n')
            {
                line[i] = ' ';
                ++i;
            }
            if (!line[i])
            {
                free(line);
                line = NULL;
                error = my_strtok(&line, &buffer, ";");
                if (error != success) return error;
                continue;
            }
        }
        free_from_delims(&line);
        int breakpoint;
        if ((error = check_comments(&line, &breakpoint)) == success) // то есть комментарий не закончился
        {
            comment = 1;
            if (line[0] != '#')
            {
                char * line_copy_1 = (char*)malloc((strlen(line) + 1) * sizeof(char));
                if (!line_copy_1) return no_memory;
                strcpy(line_copy_1, line);
                error = my_strtok(&line, &line_copy_1, "#");
                free(line_copy_1);
                if (error != success) return error;
            }
            else
            {
                free(line);
                line = NULL;
                error = my_strtok(&line, &buffer, ";");
                if (error != success) return error;
                continue;
            }
        }
        char * line_copy = (char*)malloc((strlen(line) + 1) * sizeof(char));
        if (!line_copy) return no_memory;
        strcpy(line_copy, line);
        char * string;
        error = my_assign_strtok(&string, &line_copy, settings->operations_names[OPERATIONS_COUNT - 1]);
        if (error != success)
        {
            free(buffer);
            free(line);
            free(line_copy);
            return error;
        }
        uint32_t value;
        char * variable_name;
        if (settings->operation_result_type == left)
        {
            if (is_variable(string) != success)
            {
                free(string);
                free(buffer);
                free(line);
                free(line_copy);
                return invalid_variable;
            }
            variable_name = (char*)malloc((strlen(string) + 1) * sizeof(char));
            if (!variable_name)
            {
                free(string);
                free(buffer);
                free(line);
                free(line_copy);
                return no_memory;
            }
            strcpy(variable_name, string);
            free(string);
            string = NULL;
            error = my_strtok(&string, &line_copy, settings->operations_names[OPERATIONS_COUNT - 1]);
            if (error != success)
            {
                free(buffer);
                free(line);
                free(line_copy);
                return error;
            }
            if ((error = solve_expression(settings, trie, string, &value, input_base, output_base, assign_base)) != success)
            {
                free(string);
                free(buffer);
                free(line);
                free(variable_name);
                free(line_copy);
                return error;
            }
        }
        else
        {
            char * expression = (char*)malloc((strlen(string) + 1) * sizeof(char));
            if (!expression)
            {
                free(string);
                free(buffer);
                free(line_copy);
                free(line);
                return no_memory;
            }
            strcpy(expression, string);
            free(string);
            string = NULL;
            error = my_strtok(&string, &line_copy, settings->operations_names[OPERATIONS_COUNT - 1]);
            if (error != success)
            {
                free(string);
                free(buffer);
                free(line);
                free(line_copy);
                free(expression);
                return error;
            }
            if (is_variable(string) != success)
            {
                free(string);
                free(buffer);
                free(line);
                free(line_copy);
                free(expression);
                return invalid_variable;
            }
            variable_name = (char*)malloc((strlen(string) + 1) * sizeof(char));
            if (!variable_name)
            {
                free(string);
                free(buffer);
                free(line);
                free(line_copy);
                free(expression);
                return no_memory;
            }
            strcpy(variable_name, string);
            free(string);
            string = NULL;
            if ((error = solve_expression(settings, trie, expression, &value, input_base, output_base, assign_base)) != success)
            {
                free(string);
                free(buffer);
                free(line);
                free(expression);
                free(line_copy);
                return error;
            }
            free(expression);
        }
        if ((error = Trie_insert(trie, variable_name, value)) != success)
        {
            free(string);
            free(buffer);
            free(line);
            free(variable_name);
            free(line_copy);
            return error;
        }
        free(variable_name);
        free(string);
        free(line_copy);
        free(line);
        line = NULL;
        error = my_strtok(&line, &buffer, ";");
        if (error != success) return error;
    }
    free(buffer);
    return success;
}

status read_full_file(FILE * file, char ** string)
{
    size_t size = 0, capacity = 16;
    *string = (char*)calloc(capacity, sizeof(char));
    if (!*string)
    {
        fclose(file);
        return no_memory;
    }
    char c = getc(file);
    while (c != EOF)
    {
        if (size + 1 == capacity)
        {
            capacity *= 2;
            char * tmp = (char*)realloc(*string, capacity * sizeof(char));
            if (!tmp)
            {
                free(*string);
                fclose(file);
                return no_memory;
            }
            *string = tmp;
        }
        (*string)[size] = c;
        ++size;
        c = getc(file);
    }
    (*string)[size] = 0;
    fclose(file);
    return success;
}

status interpretate(FILE * file, Current_settings_ptr settings, int input_base, int assign_base, int output_base)
{
    Trie_ptr trie = Trie_create();
    if (!trie)
    {
        fclose(file);
        free_current_settings(settings);
        return file_open_error;
    }

    char * buffer = NULL;
    status error;
    if ((error = read_full_file(file, &buffer)) != success)
    {
        free_current_settings(settings);
        Trie_free(trie);
        return error;
    }

    if ((error = scan_buffer(settings, trie, buffer, input_base, output_base, assign_base)) != success)
    {
        Trie_free(trie);
        free_current_settings(settings);
        free(buffer);
        return error;
    }
    Trie_free(trie);
    free_current_settings(settings);
    free(buffer);
    return success;
}