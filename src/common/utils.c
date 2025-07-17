#include "../../include/common/utils.h"
#include <stdlib.h>
#include <string.h>

char *my_strdup(const char *str)
{
    return str ? strcpy(malloc(strlen(str) + 1), str) : NULL;
}