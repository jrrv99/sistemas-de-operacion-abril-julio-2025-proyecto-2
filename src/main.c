#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/common/types.h"
#include "../include/common/errors.h"
#include "../include/common/logger.h"

#include "core/domain/dicotomic_tree.h"
#include "core/usecases/create_directory_structure.h"

#include "adapters/file_system/unix_file_system.h"
#include "adapters/parsers/json_parser.h"

#include "infrastructure/cli/args_parser.h"
#include "infrastructure/process/process_manager.h"

int main(int argc, char *argv[])
{
    // Initialize logger
    logger_init(LOG_INFO);

    // Parse command line arguments
    char *json_file_path = NULL;
    DirectoryCreationConfig config = {
        .root_dir = ".",
        .true_text = "si tiene",
        .false_text = "no tiene",
        .concat_mode = PREFIX_MODE,
        .use_multiple_processes = false};

    StatusCode error = parse_args(argc, argv, &json_file_path, &config);

    if (error != SUCCESS)
    {
        handle_error(error, true);
    }

    // Parse JSON file
    const JsonParserPort *json_parser = get_json_parser();
    DicotomicTree *tree = json_parser->parse_file(json_file_path);

    if (tree == NULL)
    {
        logger_error("Failed to parse JSON file: %s", json_file_path);
        free(json_file_path);
        return EXIT_FAILURE;
    }

    // Validate the tree
    if (!dicotomic_tree_validate(tree))
    {
        logger_error("The dicotomic tree is invalid");
        dicotomic_tree_free(tree);
        free(json_file_path);
        return EXIT_FAILURE;
    }

    // Create directory structure
    const FileSystemPort *file_system = get_unix_file_system();
    error = create_directory_structure(tree, &config, file_system);

    if (error != SUCCESS)
    {
        handle_error(error, false);
    }
    else
    {
        logger_info("Directory structure created successfully");
    }

    // Clean up
    dicotomic_tree_free(tree);
    free(json_file_path);
    logger_cleanup();

    return (error == SUCCESS) ? EXIT_SUCCESS : EXIT_FAILURE;
}