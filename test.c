#include "interpreter.h"

int main()
{
    char * buffer = " var = add(1, 2);\nvar2   \n = add(1,3); var3=mult(var,var2);  \n";
    Trie_ptr trie = Trie_create();
    Current_settings_ptr settings = create_current_settings();
    status error = scan_buffer(settings, trie, buffer, 10, 10, 10);
    if (error != success)
    {
        print_error(error);
        return error;
    }
    uint32_t value;
    get_value(trie, "var", &value);
    printf("var: %d\n", value);
    get_value(trie, "var2", &value);
    printf("var2: %d\n", value);
    get_value(trie, "var3", &value);
    printf("var3: %d\n\n", value);

    change_operation_result_type(settings, right);
    buffer = " add(1, 2) = var;\n   \n add(1,3) = var2; mult(var,var2)=var3;  \n";
    error = scan_buffer(settings, trie, buffer, 10, 10, 10);
    if (error != success)
    {
        print_error(error);
        return error;
    }
    get_value(trie, "var", &value);
    printf("var: %d\n", value);
    get_value(trie, "var2", &value);
    printf("var2: %d\n", value);
    get_value(trie, "var3", &value);
    printf("var3: %d\n\n", value);
    change_operation_result_type(settings, left);


    change_basic_syntax(settings, ADD, middle);
    change_basic_syntax(settings, MULT, middle);
    buffer = " var = 1 add 2;\nvar2   \n = 1 add 3; var3=var mult var2;  \n";
    error = scan_buffer(settings, trie, buffer, 10, 10, 10);
    if (error != success)
    {
        print_error(error);
        return error;
    }
    get_value(trie, "var", &value);
    printf("var: %d\n", value);
    get_value(trie, "var2", &value);
    printf("var2: %d\n", value);
    get_value(trie, "var3", &value);
    printf("var3: %d\n\n", value);

    change_operation_result_type(settings, right);
    buffer = " 1 add 2 = var;\n   \n =1 add 3=var2; var mult var2 = var3;  \n";
    error = scan_buffer(settings, trie, buffer, 10, 10, 10);
    if (error != success)
    {
        print_error(error);
        return error;
    }
    get_value(trie, "var", &value);
    printf("var: %d\n", value);
    get_value(trie, "var2", &value);
    printf("var2: %d\n", value);
    get_value(trie, "var3", &value);
    printf("var3: %d\n\n", value);
    change_operation_result_type(settings, left);

    change_basic_syntax(settings, ADD, right);
    change_basic_syntax(settings, MULT, right);
    buffer = " var = (1, 2)add;\nvar2   \n = (1,3)add; var3=(var,var2)mult;  \n";
    error = scan_buffer(settings, trie, buffer, 10, 10, 10);
    if (error != success)
    {
        print_error(error);
        return error;
    }
    get_value(trie, "var", &value);
    printf("var: %d\n", value);
    get_value(trie, "var2", &value);
    printf("var2: %d\n", value);
    get_value(trie, "var3", &value);
    printf("var3: %d\n\n", value);

    change_operation_result_type(settings, right);
    buffer = "  (1, 2)add=var;\n   \n (1,3)add=var2; (var,var2)mult = var3;  \n";
    error = scan_buffer(settings, trie, buffer, 10, 10, 10);
    if (error != success)
    {
        print_error(error);
        return error;
    }
    get_value(trie, "var", &value);
    printf("var: %d\n", value);
    get_value(trie, "var2", &value);
    printf("var2: %d\n", value);
    get_value(trie, "var3", &value);
    printf("var3: %d\n\n", value);
    change_operation_result_type(settings, left);

    Trie_free(trie);
    free_current_settings(settings);
    return success;
}