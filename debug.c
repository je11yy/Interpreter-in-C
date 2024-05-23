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

        if ((error = print_all_variables(node->childs[i], new_prefix)) != success) return error;

        free(new_prefix);
        new_prefix = NULL;
    }
    return success;
}