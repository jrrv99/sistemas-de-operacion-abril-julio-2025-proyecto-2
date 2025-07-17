#ifndef CREATE_DIRECTORY_STRUCTURE_H
#define CREATE_DIRECTORY_STRUCTURE_H

#include "../domain/dicotomic_tree.h"
#include "../ports/file_system_port.h"
#include "../../../include/common/types.h"

/**
 * @brief Configuration for directory creation
 */
typedef struct
{
    const char *root_dir;
    const char *true_text;
    const char *false_text;
    ConcatMode concat_mode;
    bool use_multiple_processes;
} DirectoryCreationConfig;

/**
 * @brief Create the directory structure for a dicotomic tree
 *
 * @param tree The dicotomic tree
 * @param config The configuration for directory creation
 * @param file_system The file system implementation
 * @return StatusCode SUCCESS if successful, an error code otherwise
 */
StatusCode create_directory_structure(
    const DicotomicTree *tree,
    const DirectoryCreationConfig *config,
    const FileSystemPort *file_system);

#endif /* CREATE_DIRECTORY_STRUCTURE_H */