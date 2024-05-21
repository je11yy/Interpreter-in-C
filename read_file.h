#ifndef RF_H
#define RF_H

#define _GNU_SOURCE
#include "errors.h"
#include "types.h"
#include "interpreter.h"
#include <string.h>
#define OP_COUNT 13

status read_file_with_settings(FILE *file, Current_settings_ptr settings);

#endif