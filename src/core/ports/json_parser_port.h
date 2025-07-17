#ifndef JSON_PARSER_PORT_H
#define JSON_PARSER_PORT_H

#include "../domain/dicotomic_tree.h"
#include "../../../include/common/types.h"

/**
 * @brief Interface for JSON parsing operations
 */
typedef struct
{
    /**
     * @brief Parse a JSON file into a dicotomic tree
     *
     * @param file_path The path of the JSON file
     * @return DicotomicTree* The parsed tree or NULL if parsing failed
     */
    DicotomicTree *(*parse_file)(const char *file_path);
} JsonParserPort;

#endif /* JSON_PARSER_PORT_H */