#include "unix_file_system.h"
#include "../../../include/common/logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

static StatusCode unix_create_directory(const char *path)
{
    // Create directory with permissions rwxr-xr-x (755)
    if (mkdir(path, 0755) != 0)
    {
        if (errno == EEXIST)
        {
            // Directory already exists, not an error
            return SUCCESS;
        }

        logger_error("Failed to create directory %s: %s", path, strerror(errno));
        return ERROR_DIRECTORY_CREATION;
    }

    return SUCCESS;
}

static StatusCode unix_create_file(const char *path)
{
    FILE *file = fopen(path, "w");
    if (!file)
    {
        logger_error("Failed to create file %s: %s", path, strerror(errno));
        return ERROR_FILE_CREATION;
    }

    fclose(file);
    return SUCCESS;
}

static bool unix_directory_exists(const char *path)
{
    struct stat st;
    if (stat(path, &st) != 0)
    {
        return false;
    }

    return S_ISDIR(st.st_mode);
}

static bool unix_file_exists(const char *path)
{
    struct stat st;
    if (stat(path, &st) != 0)
    {
        return false;
    }

    return S_ISREG(st.st_mode);
}

static const FileSystemPort unix_file_system = {
    .create_directory = unix_create_directory,
    .create_file = unix_create_file,
    .directory_exists = unix_directory_exists,
    .file_exists = unix_file_exists};

const FileSystemPort *get_unix_file_system(void)
{
    return &unix_file_system;
}