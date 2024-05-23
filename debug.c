#include "debug.h"

status print_all_variables(Trie_node_ptr node, char * prefix)
{
    if (!node) return success;
    size_t size = strlen(prefix);
    char * new_prefix = NULL;
    status error;

    if (node->is_final) printf("%s : %d\n", prefix, node->value);

    for (int i = 0; i < VARIABLE_CHARAS_COUNT; ++i)
    {
        new_prefix = (char*)calloc(size + 2, sizeof(char));
        if (!new_prefix) return no_memory;
        strcpy(new_prefix, prefix);

        char chara;
        if (i < 10) chara = i + '0';
        else if (i < 62) chara = i + 'A';
        else chara = '_';

        new_prefix[size] = chara;
        new_prefix[size + 1] = 0;

        if ((error = print_all_variables(node->childs[i], new_prefix)) != success)
        {
            free(new_prefix);
            return error;
        }
        free(new_prefix);
    }
    return success;
}

status print_variables(Trie_ptr trie)
{
    char * prefix = (char*)calloc(1, sizeof(char));
    if (!prefix) return no_memory;
    status error = success;
    if ((error = print_all_variables(trie->root, prefix)) != success) goto cleanup;

    cleanup:
        if (prefix) free(prefix);
        return error;
}

void print_memory_dump(void * ptr, size_t size)
{
    unsigned char * byte_ptr = (unsigned char *)ptr;
    for (size_t i = 0; i < size; ++i)
    {
        for (int j = 7; j >= 0; --j) printf("%d", (byte_ptr[i] >> j) & 1);
        if (i < size - 1) printf(" ");
    }
    printf("\n");
}

status print_variable_and_value(Trie_ptr trie)
{
    char * line = NULL;
    char * result = NULL;
    status error = success;
    size_t size = 0;
    if (getline(&line, &size, stdin) < 0) return no_memory;
    size = strlen(line);
    if (line[size - 1] == '\n') line[size - 1] = 0;

    Trie_node_ptr node = NULL;
    if ((error = Trie_find(trie, line, &node)) != success) goto cleanup;

    if ((error = convert_to_base(node->value, 16, &result)) != success) goto cleanup;

    printf("variable: %s\nvalue in 16 base: %d\ndump: ", line, result);
    print_memory_dump(&(node->value), sizeof(node->value));

    cleanup:
        free(line);
        if (result) free(result);
        return error;
}

status change_variable_value(Trie_ptr trie)
{
    char * line = NULL;
    status error = success;
    size_t size = 0;
    if (getline(&line, &size, stdin) < 0) return no_memory;
    size = strlen(line);
    if (line[size - 1] == '\n') line[size - 1] = 0;
    uint32_t value;
    if ((error = get_value(trie, line, &value, 16)) != success) goto cleanup;

    Trie_node_ptr node = NULL;
    if ((error = Trie_find(trie, line, &node)) != success) goto cleanup;
    node->value = value;

    cleanup:
        free(line);
        return error;
}

status insert_new_variable(Trie_ptr trie)
{
    // TODO
}

status insert_new_variable_zekendorf(Trie_ptr trie, char * name)
{
    // TODO
}

status insert_new_variable_rim(Trie_ptr trie, char * name)
{
    // TODO
}

status remove_variable(Trie_ptr trie, char * variables[], size_t size)
{
    status error = success;
    for (size_t i = 0; i < size; ++i)
    {
        Trie_node_ptr node = NULL;
        if ((error = Trie_find(trie, variables[i], &node)) != success) return error;
        node->is_final = 0;
    }
}

status debugger(Trie_ptr trie)
{
    // TODO DIALOG
    printf("\tDEBUGGER\n");
    int count = 7;
    char * functions[] = {
        "Print variable, its value and dump;",
        "Print all initialized variables;",
        "Change variable value;",
        "Insert new variable;",
        "Delete variables inserted while debugging;",
        "Stop debugging;",
        "Stop interpreter."
    };
    for (int i = 0; i < count; ++i) printf("%d. %s\n", i + 1, functions[i]);

    char * write_down = "Write down the number of function >> ";
    uint32_t number;
    while (1)
    {
        printf("%s", write_down);
    }
}