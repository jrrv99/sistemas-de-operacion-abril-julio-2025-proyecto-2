#ifndef SPECIES_H
#define SPECIES_H

#include <stdbool.h>

/**
 * @brief Represents a question-answer pair
 */
typedef struct
{
    char *question;
    bool answer;
} QuestionAnswer;

/**
 * @brief Represents a species with its characteristics
 */
typedef struct
{
    char *name;
    QuestionAnswer *characteristics;
    int num_characteristics;
} Species;

/**
 * @brief Create a new species
 *
 * @param name The name of the species
 * @return Species* The created species or NULL if memory allocation failed
 */
Species *species_create(const char *name);

/**
 * @brief Add a characteristic to a species
 *
 * @param species The species
 * @param question The question
 * @param answer The answer
 * @return bool true if successful, false otherwise
 */
bool species_add_characteristic(Species *species, const char *question, bool answer);

/**
 * @brief Free the memory allocated for a species
 *
 * @param species The species to free
 */
void species_free(Species *species);

/**
 * @brief Check if the species follows the expected question order
 *
 * @param species The species to check
 * @param expected_questions The expected questions in order
 * @param num_expected_questions The number of expected questions
 * @return bool true if the species follows the expected order, false otherwise
 */
bool species_follows_question_order(const Species *species, const char **expected_questions, int num_expected_questions);

#endif /* SPECIES_H */