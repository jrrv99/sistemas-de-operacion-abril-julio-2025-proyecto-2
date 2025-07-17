#include "../../include/common/errors.h"
#include "../../include/common/logger.h"
#include <stdlib.h>

const char *get_error_message(StatusCode error_code)
{
    switch (error_code)
    {
    case SUCCESS:
        return "Success";
    case ERROR_FILE_NOT_FOUND:
        return "File not found";
    case ERROR_INVALID_JSON:
        return "Invalid JSON format";
    case ERROR_MEMORY_ALLOCATION:
        return "Memory allocation failed";
    case ERROR_DIRECTORY_CREATION:
        return "Failed to create directory";
    case ERROR_FILE_CREATION:
        return "Failed to create file";
    case ERROR_INVALID_ARGUMENTS:
        return "Invalid arguments";
    default:
        return "Unknown error";
    }
}

void handle_error(StatusCode error_code, bool should_exit)
{
    if (error_code == SUCCESS)
    {
        return;
    }

    logger_error("Error: %s", get_error_message(error_code));

    if (should_exit)
    {
        exit(error_code);
    }
}