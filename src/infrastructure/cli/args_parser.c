#include "args_parser.h"
#include "../../../include/common/utils.h"
#include "../../../include/common/logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

void print_usage(void)
{
    printf("Usage: dicotodir <clave> [-d|--dir <raiz>] [-t|--true <p1>] [-f|--false <p2>] [-p|--pre] [-s|--suf]\n");
    printf("Options:\n");
    printf("  <clave>              JSON file containing the dicotomic key\n");
    printf("  -d, --dir <raiz>     Directory where to create the directory structure (default: current directory)\n");
    printf("  -t, --true <p1>      Text to concatenate to questions for true answers (default: \"si tiene\")\n");
    printf("  -f, --false <p2>     Text to concatenate to questions for false answers (default: \"no tiene\")\n");
    printf("  -p, --pre            Concatenate texts as prefixes (default: active)\n");
    printf("  -s, --suf            Concatenate texts as suffixes (default: inactive)\n");
    printf("  -m, --multi          Use multiple processes to create the directory structure\n");
    printf("  -h, --help           Show this help message\n");
}

StatusCode parse_args(
    int argc,
    char *argv[],
    char **json_file_path,
    DirectoryCreationConfig *config)
{
    // Define long options
    static struct option long_options[] = {
        {"dir", required_argument, 0, 'd'},
        {"true", required_argument, 0, 't'},
        {"false", required_argument, 0, 'f'},
        {"pre", no_argument, 0, 'p'},
        {"suf", no_argument, 0, 's'},
        {"multi", no_argument, 0, 'm'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}};

    // Set default values
    config->root_dir = ".";
    config->true_text = "si tiene";
    config->false_text = "no tiene";
    config->concat_mode = PREFIX_MODE;
    config->use_multiple_processes = false;

    int option_index = 0;
    int c;

    // Parse options
    while ((c = getopt_long(argc, argv, "d:t:f:psmh", long_options, &option_index)) != -1)
    {
        switch (c)
        {
        case 'd':
            config->root_dir = optarg;
            break;
        case 't':
            config->true_text = optarg;
            break;
        case 'f':
            config->false_text = optarg;
            break;
        case 'p':
            config->concat_mode = PREFIX_MODE;
            break;
        case 's':
            config->concat_mode = SUFFIX_MODE;
            break;
        case 'm':
            config->use_multiple_processes = true;
            break;
        case 'h':
            print_usage();
            return ERROR_INVALID_ARGUMENTS;
        case '?':
            // getopt_long already printed an error message
            print_usage();
            return ERROR_INVALID_ARGUMENTS;
        default:
            print_usage();
            return ERROR_INVALID_ARGUMENTS;
        }
    }

    // Check if both -p and -s are specified
    if (config->concat_mode == PREFIX_MODE && optind < argc && strcmp(argv[optind - 1], "-s") == 0)
    {
        config->concat_mode = BOTH_MODES;
    }

    // Check if JSON file is specified
    if (optind >= argc)
    {
        logger_error("No JSON file specified");
        print_usage();
        return ERROR_INVALID_ARGUMENTS;
    }

    // Get JSON file path
    *json_file_path = my_strdup(argv[optind]);
    if (!*json_file_path)
    {
        logger_error("Failed to allocate memory for JSON file path");
        return ERROR_MEMORY_ALLOCATION;
    }

    return SUCCESS;
}