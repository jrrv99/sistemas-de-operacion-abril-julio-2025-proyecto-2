#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include "../../core/ports/json_parser_port.h"

/**
 * @brief Get the JSON parser implementation
 *
 * @return const JsonParserPort* The JSON parser implementation
 */
const JsonParserPort *get_json_parser(void);

#endif /* JSON_PARSER_H */