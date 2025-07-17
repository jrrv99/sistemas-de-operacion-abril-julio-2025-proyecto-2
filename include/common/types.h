#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

/**
 * @brief Error codes for the application
 */
typedef enum
{
    SUCCESS = 0,
    ERROR_FILE_NOT_FOUND,
    ERROR_INVALID_JSON,
    ERROR_MEMORY_ALLOCATION,
    ERROR_DIRECTORY_CREATION,
    ERROR_FILE_CREATION,
    ERROR_INVALID_ARGUMENTS
} StatusCode;

/**
 * @brief Concatenation mode for directory names
 */
typedef enum
{
    PREFIX_MODE,
    SUFFIX_MODE,
    BOTH_MODES
} ConcatMode;

#endif /* TYPES_H */