#ifndef ERRORS_H
#define ERRORS_H

#include "types.h"

/**
 * @brief Get error message for a given error code
 *
 * @param error_code The error code
 * @return const char* The error message
 */
const char *get_error_message(StatusCode error_code);

/**
 * @brief Handle error and exit if necessary
 *
 * @param error_code The error code
 * @param should_exit Whether to exit the program
 */
void handle_error(StatusCode error_code, bool should_exit);

#endif /* ERRORS_H */