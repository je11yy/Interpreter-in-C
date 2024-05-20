#include "interpreter.h"

int main()
{
    char * buffer = "var=add(1,2);";
    Trie_ptr trie = Trie_create();
    status error = scan_buffer(trie, buffer, 10, 10, 10);
    if (error != success) printf("ERROR\n");
    uint32_t value;
    get_value(trie, "var", &value);
    printf("var: %d\n", value);
    Trie_free(trie);
    return success;
}