#include "json_parser.h"
#include "../../../include/common/utils.h"
#include "../../../include/common/logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Simple JSON parser implementation
// In a real project, you would use a library like cJSON or jansson

typedef enum
{
    TOKEN_NONE,
    TOKEN_OBJECT_START,
    TOKEN_OBJECT_END,
    TOKEN_ARRAY_START,
    TOKEN_ARRAY_END,
    TOKEN_STRING,
    TOKEN_NUMBER,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_NULL,
    TOKEN_COLON,
    TOKEN_COMMA,
    TOKEN_EOF
} TokenType;

typedef struct
{
    TokenType type;
    char *value;
} Token;

typedef struct
{
    const char *json;
    size_t pos;
    size_t len;
    Token current_token;
} JsonParser;

// Forward declarations
static void parser_init(JsonParser *parser, const char *json);
static void parser_free(JsonParser *parser);
static void parser_next_token(JsonParser *parser);
static bool parser_expect(JsonParser *parser, TokenType type);
static char *parser_parse_string(JsonParser *parser);
static bool parser_parse_boolean(JsonParser *parser);
static DicotomicTree *parser_parse_tree(JsonParser *parser);
static Species *parser_parse_species(JsonParser *parser, const char *name);

static DicotomicTree *parse_json_file(const char *file_path)
{
    FILE *file = fopen(file_path, "r");
    if (!file)
    {
        logger_error("Failed to open file: %s", file_path);
        return NULL;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Read file content
    char *json = malloc(file_size + 1);
    if (!json)
    {
        logger_error("Failed to allocate memory for file content");
        fclose(file);
        return NULL;
    }

    size_t read_size = fread(json, 1, file_size, file);
    json[read_size] = '\0';
    fclose(file);

    // Parse JSON
    JsonParser parser;
    parser_init(&parser, json);
    DicotomicTree *tree = parser_parse_tree(&parser);

    // Clean up
    parser_free(&parser);
    free(json);

    return tree;
}

static void parser_init(JsonParser *parser, const char *json)
{
    parser->json = json;
    parser->pos = 0;
    parser->len = strlen(json);
    parser->current_token.type = TOKEN_NONE;
    parser->current_token.value = NULL;

    parser_next_token(parser);
}

static void parser_free(JsonParser *parser)
{
    if (parser->current_token.value)
    {
        free(parser->current_token.value);
        parser->current_token.value = NULL;
    }
}

static void parser_next_token(JsonParser *parser)
{
    // Free previous token value
    if (parser->current_token.value)
    {
        free(parser->current_token.value);
        parser->current_token.value = NULL;
    }

    // Skip whitespace
    while (parser->pos < parser->len && isspace(parser->json[parser->pos]))
    {
        parser->pos++;
    }

    // Check for EOF
    if (parser->pos >= parser->len)
    {
        parser->current_token.type = TOKEN_EOF;
        return;
    }

    // Parse token
    char c = parser->json[parser->pos];
    switch (c)
    {
    case '{':
        parser->current_token.type = TOKEN_OBJECT_START;
        parser->pos++;
        break;
    case '}':
        parser->current_token.type = TOKEN_OBJECT_END;
        parser->pos++;
        break;
    case '[':
        parser->current_token.type = TOKEN_ARRAY_START;
        parser->pos++;
        break;
    case ']':
        parser->current_token.type = TOKEN_ARRAY_END;
        parser->pos++;
        break;
    case ':':
        parser->current_token.type = TOKEN_COLON;
        parser->pos++;
        break;
    case ',':
        parser->current_token.type = TOKEN_COMMA;
        parser->pos++;
        break;
    case '"':
        parser->current_token.type = TOKEN_STRING;
        parser->current_token.value = parser_parse_string(parser);
        break;
    case 't':
        if (parser->pos + 3 < parser->len &&
            parser->json[parser->pos + 1] == 'r' &&
            parser->json[parser->pos + 2] == 'u' &&
            parser->json[parser->pos + 3] == 'e')
        {
            parser->current_token.type = TOKEN_TRUE;
            parser->pos += 4;
        }
        else
        {
            logger_error("Invalid token at position %zu", parser->pos);
            parser->current_token.type = TOKEN_NONE;
        }
        break;
    case 'f':
        if (parser->pos + 4 < parser->len &&
            parser->json[parser->pos + 1] == 'a' &&
            parser->json[parser->pos + 2] == 'l' &&
            parser->json[parser->pos + 3] == 's' &&
            parser->json[parser->pos + 4] == 'e')
        {
            parser->current_token.type = TOKEN_FALSE;
            parser->pos += 5;
        }
        else
        {
            logger_error("Invalid token at position %zu", parser->pos);
            parser->current_token.type = TOKEN_NONE;
        }
        break;
    case 'n':
        if (parser->pos + 3 < parser->len &&
            parser->json[parser->pos + 1] == 'u' &&
            parser->json[parser->pos + 2] == 'l' &&
            parser->json[parser->pos + 3] == 'l')
        {
            parser->current_token.type = TOKEN_NULL;
            parser->pos += 4;
        }
        else
        {
            logger_error("Invalid token at position %zu", parser->pos);
            parser->current_token.type = TOKEN_NONE;
        }
        break;
    default:
        if (isdigit(c) || c == '-')
        {
            // Parse number (simplified)
            parser->current_token.type = TOKEN_NUMBER;
            size_t start = parser->pos;
            while (parser->pos < parser->len &&
                   (isdigit(parser->json[parser->pos]) ||
                    parser->json[parser->pos] == '.' ||
                    parser->json[parser->pos] == '-' ||
                    parser->json[parser->pos] == '+' ||
                    parser->json[parser->pos] == 'e' ||
                    parser->json[parser->pos] == 'E'))
            {
                parser->pos++;
            }
            size_t len = parser->pos - start;
            parser->current_token.value = malloc(len + 1);
            if (parser->current_token.value)
            {
                strncpy(parser->current_token.value, parser->json + start, len);
                parser->current_token.value[len] = '\0';
            }
        }
        else
        {
            logger_error("Invalid token at position %zu: %c", parser->pos, c);
            parser->current_token.type = TOKEN_NONE;
            parser->pos++;
        }
        break;
    }
}

static bool parser_expect(JsonParser *parser, TokenType type)
{
    if (parser->current_token.type != type)
    {
        logger_error("Expected token type %d, got %d", type, parser->current_token.type);
        return false;
    }

    parser_next_token(parser);
    return true;
}

static char *parser_parse_string(JsonParser *parser)
{
    // Skip opening quote
    parser->pos++;

    size_t start = parser->pos;
    size_t len = 0;

    // Find closing quote
    while (parser->pos < parser->len && parser->json[parser->pos] != '"')
    {
        // Handle escape sequences
        if (parser->json[parser->pos] == '\\' && parser->pos + 1 < parser->len)
        {
            parser->pos += 2;
        }
        else
        {
            parser->pos++;
        }
        len++;
    }

    // Skip closing quote
    if (parser->pos < parser->len)
    {
        parser->pos++;
    }

    // Allocate and copy string
    char *str = malloc(len + 1);
    if (!str)
    {
        logger_error("Failed to allocate memory for string");
        return NULL;
    }

    // Copy and unescape string
    size_t j = 0;
    for (size_t i = start; i < start + len; i++)
    {
        if (parser->json[i] == '\\' && i + 1 < parser->len)
        {
            i++;
            switch (parser->json[i])
            {
            case 'n':
                str[j++] = '\n';
                break;
            case 'r':
                str[j++] = '\r';
                break;
            case 't':
                str[j++] = '\t';
                break;
            case 'b':
                str[j++] = '\b';
                break;
            case 'f':
                str[j++] = '\f';
                break;
            case '\\':
                str[j++] = '\\';
                break;
            case '/':
                str[j++] = '/';
                break;
            case '"':
                str[j++] = '"';
                break;
            default:
                str[j++] = parser->json[i];
                break;
            }
        }
        else
        {
            str[j++] = parser->json[i];
        }
    }

    str[j] = '\0';
    return str;
}

static bool parser_parse_boolean(JsonParser *parser)
{
    bool value = (parser->current_token.type == TOKEN_TRUE);
    parser_next_token(parser);
    return value;
}

static DicotomicTree *parser_parse_tree(JsonParser *parser)
{
    // Expect object start
    if (!parser_expect(parser, TOKEN_OBJECT_START))
    {
        return NULL;
    }

    // Parse tree name
    if (parser->current_token.type != TOKEN_STRING)
    {
        logger_error("Expected tree name as string");
        return NULL;
    }

    char *tree_name = my_strdup(parser->current_token.value);
    parser_next_token(parser);

    // Expect colon
    if (!parser_expect(parser, TOKEN_COLON))
    {
        free(tree_name);
        return NULL;
    }

    // Expect array start
    if (!parser_expect(parser, TOKEN_ARRAY_START))
    {
        free(tree_name);
        return NULL;
    }

    // Create tree
    DicotomicTree *tree = dicotomic_tree_create(tree_name);
    free(tree_name);

    if (!tree)
    {
        logger_error("Failed to create dicotomic tree");
        return NULL;
    }

    // Parse species
    while (parser->current_token.type == TOKEN_OBJECT_START)
    {
        parser_next_token(parser);

        // Parse species name
        if (parser->current_token.type != TOKEN_STRING)
        {
            logger_error("Expected species name as string");
            dicotomic_tree_free(tree);
            return NULL;
        }

        char *species_name = my_strdup(parser->current_token.value);
        parser_next_token(parser);

        // Expect colon
        if (!parser_expect(parser, TOKEN_COLON))
        {
            free(species_name);
            dicotomic_tree_free(tree);
            return NULL;
        }

        // Parse species characteristics
        Species *species = parser_parse_species(parser, species_name);
        free(species_name);

        if (!species)
        {
            logger_error("Failed to parse species");
            dicotomic_tree_free(tree);
            return NULL;
        }

        // Add species to tree
        if (!dicotomic_tree_add_species(tree, species))
        {
            logger_error("Failed to add species to tree");
            species_free(species);
            dicotomic_tree_free(tree);
            return NULL;
        }

        // Expect object end
        if (!parser_expect(parser, TOKEN_OBJECT_END))
        {
            dicotomic_tree_free(tree);
            return NULL;
        }

        // Expect comma or array end
        if (parser->current_token.type == TOKEN_COMMA)
        {
            parser_next_token(parser);
        }
        else if (parser->current_token.type == TOKEN_ARRAY_END)
        {
            break;
        }
        else
        {
            logger_error("Expected comma or array end");
            dicotomic_tree_free(tree);
            return NULL;
        }
    }

    // Expect array end
    if (!parser_expect(parser, TOKEN_ARRAY_END))
    {
        dicotomic_tree_free(tree);
        return NULL;
    }

    // Expect object end
    if (!parser_expect(parser, TOKEN_OBJECT_END))
    {
        dicotomic_tree_free(tree);
        return NULL;
    }

    // Extract questions
    if (!dicotomic_tree_extract_questions(tree))
    {
        logger_error("Failed to extract questions from tree");
        dicotomic_tree_free(tree);
        return NULL;
    }

    return tree;
}

static Species *parser_parse_species(JsonParser *parser, const char *name)
{
    // Expect array start
    if (!parser_expect(parser, TOKEN_ARRAY_START))
    {
        return NULL;
    }

    // Create species
    Species *species = species_create(name);
    if (!species)
    {
        logger_error("Failed to create species");
        return NULL;
    }

    // Parse characteristics
    while (parser->current_token.type == TOKEN_OBJECT_START)
    {
        parser_next_token(parser);

        // Parse question
        if (parser->current_token.type != TOKEN_STRING)
        {
            logger_error("Expected question as string");
            species_free(species);
            return NULL;
        }

        char *question = my_strdup(parser->current_token.value);
        parser_next_token(parser);

        // Expect colon
        if (!parser_expect(parser, TOKEN_COLON))
        {
            free(question);
            species_free(species);
            return NULL;
        }

        // Parse answer
        bool answer;
        if (parser->current_token.type == TOKEN_TRUE || parser->current_token.type == TOKEN_FALSE)
        {
            answer = parser_parse_boolean(parser);
        }
        else
        {
            logger_error("Expected boolean value");
            free(question);
            species_free(species);
            return NULL;
        }

        // Add characteristic to species
        if (!species_add_characteristic(species, question, answer))
        {
            logger_error("Failed to add characteristic to species");
            free(question);
            species_free(species);
            return NULL;
        }

        free(question);

        // Expect object end
        if (!parser_expect(parser, TOKEN_OBJECT_END))
        {
            species_free(species);
            return NULL;
        }

        // Expect comma or array end
        if (parser->current_token.type == TOKEN_COMMA)
        {
            parser_next_token(parser);
        }
        else if (parser->current_token.type == TOKEN_ARRAY_END)
        {
            break;
        }
        else
        {
            logger_error("Expected comma or array end");
            species_free(species);
            return NULL;
        }
    }

    // Expect array end
    if (!parser_expect(parser, TOKEN_ARRAY_END))
    {
        species_free(species);
        return NULL;
    }

    return species;
}

static const JsonParserPort json_parser = {
    .parse_file = parse_json_file};

const JsonParserPort *get_json_parser(void)
{
    return &json_parser;
}