#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H

#include "../../core/usecases/create_directory_structure.h"
#include "../../../include/common/types.h"

/**
 * @brief Parse command line arguments
 *
 * @param argc The argument count
 * @param argv The argument values
 * @param json_file_path Pointer to store the JSON file path
 * @param config Pointer to store the directory creation configuration
 * @return StatusCode SUCCESS if successful, an error code otherwise
 */
StatusCode parse_args(
    int argc,
    char *argv[],
    char **json_file_path,
    DirectoryCreationConfig *config);

/**
 * @brief Print the usage information
 */
void print_usage(void);

#endif /* ARGS_PARSER_H */