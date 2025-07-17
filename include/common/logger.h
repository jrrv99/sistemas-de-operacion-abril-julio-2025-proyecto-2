#ifndef LOGGER_H
#define LOGGER_H

#include <stdarg.h>

typedef enum
{
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

/**
 * @brief Initialize the logger
 *
 * @param level The minimum log level to display
 */
void logger_init(LogLevel level);

/**
 * @brief Log a message with the given level
 *
 * @param level The log level
 * @param format The format string
 * @param ... The format arguments
 */
void logger_log(LogLevel level, const char *format, ...);

/**
 * @brief Shorthand for logger_log(LOG_DEBUG, ...)
 */
void logger_debug(const char *format, ...);

/**
 * @brief Shorthand for logger_log(LOG_INFO, ...)
 */
void logger_info(const char *format, ...);

/**
 * @brief Shorthand for logger_log(LOG_WARNING, ...)
 */
void logger_warning(const char *format, ...);

/**
 * @brief Shorthand for logger_log(LOG_ERROR, ...)
 */
void logger_error(const char *format, ...);

/**
 * @brief Clean up the logger
 */
void logger_cleanup(void);

#endif /* LOGGER_H */