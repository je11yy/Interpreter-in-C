#include "operations.h"

status convert_to_decimal_base(char * to_convert, int original_base, uint32_t * result)
{
    if (!to_convert) return invalid_function_argument;
    status error;
    if ((error = check_if_in_base(to_convert, original_base)) != success) return error;

    uint32_t preresult = 0;
    int size = strlen(to_convert);
    int power = 1;
    for (int i = size - 1; i >= 0; --i)
    {
        if (isdigit(to_convert[i])) preresult += (power) * (to_convert[i] - '0');
        else preresult += (power) * (toupper(to_convert[i]) - 'A' + 10);
        power *= 10;
    }
    *result = preresult;
    return success;
}

status convert_to_base(uint32_t to_convert, int to_base, char ** result)
{
    if (to_base < 2 || to_base > 36) return invalid_base;
    int size = get_symbols_count(to_convert, to_base);
    *result = (char*)malloc((size + 1) * sizeof(char));
    if (!*result) return no_memory;
    (*result)[size] = 0;
    int i = size - 1;
    while (i)
    {
        int remainder = to_convert % to_base;
        if (remainder < 10) (*result)[i] = remainder + '0';
        else (*result)[i] = remainder - 10 + 'A';
        to_convert /= to_base;
        --i;
    }
    return success;
}

status get_value(Trie_ptr trie, char * name, uint32_t * result, int original_base)
{
    Trie_node_ptr node = NULL;
    uint32_t value;
    status error = Trie_find(trie, name, &node);

    if (error == success) value = node->value;
    else if (error == cannot_find && is_number(name) == success)
    {
        error = convert_to_decimal_base(name, original_base, &value);
        if (error != success) return error;
    }
    else return error;
    *result = value;
    return success;
}

status not_operation(Trie_ptr trie, uint32_t value, uint32_t * result)
{
    *result = ~value;
    return success;
}

status output_operation(Trie_ptr trie, uint32_t value, int output_base)
{
    // TODO to base
    printf("%ud\n", value);
    return success;
}

status input_operation(Trie_ptr trie, int input_base, uint32_t * result)
{
    char * line = NULL;
    size_t size = 0;
    if (getline(&line, &size, stdin) < 0) return invalid_variable;
    if (line[size - 1] == '\n') line[size - 1] = 0;

    status error;
    uint32_t value;
    if ((error = convert_to_decimal_base(line, input_base, result)) != success)
    {
        free(line);
        return error;
    }
    return success;
}

status add_operation(Trie_ptr trie, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    *result = value_1 + value_2;
    return success;
}

status mult_operation(Trie_ptr trie, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    *result = value_1 * value_2;
    return success;
}

status sub_operation(Trie_ptr trie, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    *result = value_1 - value_2;
    return success;
}

uint32_t fast_mod_pow(uint32_t base, uint32_t exponent, uint32_t modulus) {
  uint32_t result = 1;
  base %= modulus;

  while (exponent > 0) {
    if (exponent & 1) {
      result = (result * base) % modulus;
    }
    exponent >>= 1;
    base = (base * base) % modulus;
  }
  return result;
}

status pow_operation(Trie_ptr trie, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    *result = fast_mod_pow(value_1, value_2, (1 << 32));
    return success;
}

status div_operation(Trie_ptr trie, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    *result = value_1 / value_2;
    return success;
}

status rem_operation(Trie_ptr trie, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    *result = value_1 % value_2;
    return success;
}

status xor_operation(Trie_ptr trie, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    *result = value_1 ^ value_2;
    return success;
}

status and_operation(Trie_ptr trie, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    *result = value_1 & value_2;
    return success;
}

status or_operation(Trie_ptr trie, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    *result = value_1 | value_2;
    return success;
}