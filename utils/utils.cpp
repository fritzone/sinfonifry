#include "utils.h"
#include <string.h>
#include <stdlib.h>

void copy_string_to_char(const std::string &str, char *& dest)
{
    dest = (char*)calloc(str.length() + 1, sizeof(char));
    strncpy(dest, str.c_str(), str.length());
}
