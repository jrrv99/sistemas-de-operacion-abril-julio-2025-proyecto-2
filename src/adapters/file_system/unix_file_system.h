#ifndef UNIX_FILE_SYSTEM_H
#define UNIX_FILE_SYSTEM_H

#include "../../core/ports/file_system_port.h"

/**
 * @brief Get the Unix file system implementation
 *
 * @return const FileSystemPort* The file system implementation
 */
const FileSystemPort *get_unix_file_system(void);

#endif /* UNIX_FILE_SYSTEM_H */