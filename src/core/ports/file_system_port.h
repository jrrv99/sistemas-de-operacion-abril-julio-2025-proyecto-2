#ifndef FILE_SYSTEM_PORT_H
#define FILE_SYSTEM_PORT_H

#include <stdbool.h>
#include "../../../include/common/types.h"

/**
 * @brief Interface for file system operations
 */
typedef struct
{
    /**
     * @brief Create a directory
     *
     * @param path The path of the directory to create
     * @return StatusCode SUCCESS if successful, an error code otherwise
     */
    StatusCode (*create_directory)(const char *path);

    /**
     * @brief Create an empty file
     *
     * @param path The path of the file to create
     * @return StatusCode SUCCESS if successful, an error code otherwise
     */
    StatusCode (*create_file)(const char *path);

    /**
     * @brief Check if a directory exists
     *
     * @param path The path of the directory to check
     * @return bool true if the directory exists, false otherwise
     */
    bool (*directory_exists)(const char *path);

    /**
     * @brief Check if a file exists
     *
     * @param path The path of the file to check
     * @return bool true if the file exists, false otherwise
     */
    bool (*file_exists)(const char *path);
} FileSystemPort;

#endif /* FILE_SYSTEM_PORT_H */