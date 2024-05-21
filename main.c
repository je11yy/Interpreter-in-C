#include "read_file.h"

int main(int argc, char *argv[]){
    FILE *file = fopen(argv[1], "r");
    if(!file)
    {
        print_error(file_open_error);
        return file_open_error;
    }
    Current_settings_ptr settings = create_current_settings();
    status error = read_file_with_settings(file, settings);
    if (error != success)
    {
        print_error(error);
        return error;
    }

    for(int i = 0; i < 13; i++) printf("%s %d\n", settings->operations_names[i], settings->basic_syntax[i]);

    printf("Assignment: %d\n", settings->operation_result_type);
}