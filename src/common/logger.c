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

static void logger_logv(LogLevel level, const char *format, va_list args)
{
    if (level < current_level)
    {
        return;
    }

    const char *level_str;
    const char *color_code;
    FILE *output = stdout;

    // Thanks to Juan and Amin for explaining how to add colors with ANSI
    switch (level)
    {
    case LOG_DEBUG:
        level_str = "DEBUG";
        color_code = "\033[36m"; // Cyan
        break;
    case LOG_INFO:
        level_str = "INFO";
        color_code = "\033[32m"; // Verde
        break;
    case LOG_WARNING:
        level_str = "WARNING";
        color_code = "\033[33m"; // Amarillo
        output = stderr;
        break;
    case LOG_ERROR:
        level_str = "ERROR";
        color_code = "\033[31m"; // Rojo
        output = stderr;
        break;
    default:
        level_str = "UNKNOWN";
        color_code = "\033[0m"; // Reset
        break;
    }

    // Get current time
    time_t now = time(NULL);
    const struct tm *tm_info = localtime(&now);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);

    // Print log header with color
    fprintf(output, "%s[%s] [%s]%s ", color_code, time_str, level_str, "\033[0m");

    // Print log message
    vfprintf(output, format, args);

    fprintf(output, "\n");
}

void logger_log(LogLevel level, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logger_logv(level, format, args);
    va_end(args);
}

void logger_debug(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logger_logv(LOG_DEBUG, format, args);
    va_end(args);
}

void logger_info(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logger_logv(LOG_INFO, format, args);
    va_end(args);
}

void logger_warning(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logger_logv(LOG_WARNING, format, args);
    va_end(args);
}

void logger_error(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logger_logv(LOG_ERROR, format, args);
    va_end(args);
}

void logger_cleanup(void)
{
    // Nothing to clean up in this implementation
}