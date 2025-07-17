#include "species.h"
#include "../../../include/common/utils.h"
#include "../../../include/common/logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Species *species_create(const char *name)
{
    Species *species = malloc(sizeof(Species));
    if (!species)
    {
        logger_error("Failed to allocate memory for species");
        return NULL;
    }

    species->name = my_strdup(name);
    if (!species->name)
    {
        logger_error("Failed to allocate memory for species name");
        free(species);
        return NULL;
    }

    species->characteristics = NULL;
    species->num_characteristics = 0;

    return species;
}

bool species_add_characteristic(Species *species, const char *question, bool answer)
{
    if (!species || !question)
    {
        logger_error("Invalid species or question");
        return false;
    }

    // Resize characteristics array
    QuestionAnswer *new_characteristics = realloc(
        species->characteristics,
        (species->num_characteristics + 1) * sizeof(QuestionAnswer));

    if (!new_characteristics)
    {
        logger_error("Failed to resize characteristics array");
        return false;
    }

    species->characteristics = new_characteristics;

    // Add new characteristic
    QuestionAnswer *characteristic = &species->characteristics[species->num_characteristics];
    characteristic->question = my_strdup(question);
    if (!characteristic->question)
    {
        logger_error("Failed to allocate memory for question");
        return false;
    }

    characteristic->answer = answer;
    species->num_characteristics++;

    return true;
}

void species_free(Species *species)
{
    if (!species)
    {
        return;
    }

    // Free name
    free(species->name);

    // Free characteristics
    for (int i = 0; i < species->num_characteristics; i++)
    {
        free(species->characteristics[i].question);
    }
    free(species->characteristics);

    // Free species
    free(species);
}

bool species_follows_question_order(
    const Species *species,
    const char **expected_questions,
    int num_expected_questions)
{
    if (!species || !expected_questions || num_expected_questions <= 0)
    {
        logger_error("Invalid parameters for species_follows_question_order");
        return false;
    }

    // Check that each characteristic follows the expected order
    int expected_index = 0;

    for (int i = 0; i < species->num_characteristics; i++)
    {
        const char *question = species->characteristics[i].question;
        bool found = false;

        // Find the question in the expected questions
        for (int j = expected_index; j < num_expected_questions; j++)
        {
            if (strcmp(question, expected_questions[j]) == 0)
            {
                // Question found at position j
                if (j != expected_index)
                {
                    // Question found but out of order
                    logger_warning(
                        "Question '%s' for species '%s' is out of order (expected at position %d, found at %d)",
                        question, species->name, expected_index, j);
                    return false;
                }

                found = true;
                expected_index++;
                break;
            }
        }

        if (!found)
        {
            // Question not found in expected questions
            logger_warning(
                "Question '%s' for species '%s' not found in expected questions",
                question, species->name);
            return false;
        }
    }

    return true;
}