#include "../../include/common/logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

static LogLevel current_level = LOG_INFO;

void logger_init(LogLevel level)
{
    current_level = level;
}

void logger_log(LogLevel level, const char *format, ...)
{
    if (level < current_level)
    {
        return;
    }

    const char *level_str;
    FILE *output = stdout;

    switch (level)
    {
    case LOG_DEBUG:
        level_str = "DEBUG";
        break;
    case LOG_INFO:
        level_str = "INFO";
        break;
    case LOG_WARNING:
        level_str = "WARNING";
        output = stderr;
        break;
    case LOG_ERROR:
        level_str = "ERROR";
        output = stderr;
        break;
    default:
        level_str = "UNKNOWN";
        break;
    }

    // Get current time
    time_t now = time(NULL);
    const struct tm *tm_info = localtime(&now);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);

    // Print log header
    fprintf(output, "[%s] [%s] ", time_str, level_str);

    // Print log message
    va_list args;
    va_start(args, format);
    vfprintf(output, format, args);
    va_end(args);

    fprintf(output, "\n");
}

void logger_debug(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logger_log(LOG_DEBUG, format, args);
    va_end(args);
}

void logger_info(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logger_log(LOG_INFO, format, args);
    va_end(args);
}

void logger_warning(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logger_log(LOG_WARNING, format, args);
    va_end(args);
}

void logger_error(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logger_log(LOG_ERROR, format, args);
    va_end(args);
}

void logger_cleanup(void)
{
    // Nothing to clean up in this implementation
}