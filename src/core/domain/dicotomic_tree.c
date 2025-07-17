#include "dicotomic_tree.h"
#include "../../../include/common/utils.h"
#include "../../../include/common/logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DicotomicTree *dicotomic_tree_create(const char *name)
{
    DicotomicTree *tree = malloc(sizeof(DicotomicTree));
    if (!tree)
    {
        logger_error("Failed to allocate memory for dicotomic tree");
        return NULL;
    }

    tree->name = my_strdup(name);
    if (!tree->name)
    {
        logger_error("Failed to allocate memory for tree name");
        free(tree);
        return NULL;
    }

    tree->species = NULL;
    tree->num_species = 0;
    tree->questions = NULL;
    tree->num_questions = 0;

    return tree;
}

bool dicotomic_tree_add_species(DicotomicTree *tree, Species *species)
{
    if (!tree || !species)
    {
        logger_error("Invalid tree or species");
        return false;
    }

    // Resize species array
    Species **new_species = realloc(tree->species, (tree->num_species + 1) * sizeof(Species *));
    if (!new_species)
    {
        logger_error("Failed to resize species array");
        return false;
    }

    tree->species = new_species;
    tree->species[tree->num_species] = species;
    tree->num_species++;

    return true;
}

/**
 * @brief Check if a question is already in the array
 *
 * @param questions The array of questions
 * @param num_questions The number of questions in the array
 * @param question The question to check
 * @return bool true if the question is in the array, false otherwise
 */
static bool is_question_in_array(const char **questions, int num_questions, const char *question)
{
    for (int i = 0; i < num_questions; i++)
    {
        if (strcmp(questions[i], question) == 0)
        {
            return true;
        }
    }

    return false;
}

bool dicotomic_tree_extract_questions(DicotomicTree *tree)
{
    if (!tree || tree->num_species == 0)
    {
        logger_error("Invalid tree or no species");
        return false;
    }

    // Free existing questions
    if (tree->questions)
    {
        for (int i = 0; i < tree->num_questions; i++)
        {
            free(tree->questions[i]);
        }
        free(tree->questions);
        tree->questions = NULL;
        tree->num_questions = 0;
    }

    // Count unique questions
    int max_questions = 0;
    for (int i = 0; i < tree->num_species; i++)
    {
        max_questions += tree->species[i]->num_characteristics;
    }

    // Allocate temporary array for unique questions
    const char **unique_questions = malloc(max_questions * sizeof(char *));
    if (!unique_questions)
    {
        logger_error("Failed to allocate memory for unique questions");
        return false;
    }

    int num_unique_questions = 0;

    // Extract unique questions in order
    for (int i = 0; i < tree->num_species; i++)
    {
        Species *species = tree->species[i];

        for (int j = 0; j < species->num_characteristics; j++)
        {
            const char *question = species->characteristics[j].question;

            if (!is_question_in_array(unique_questions, num_unique_questions, question))
            {
                unique_questions[num_unique_questions] = question;
                num_unique_questions++;
            }
        }
    }

    // Allocate and copy unique questions
    tree->questions = malloc(num_unique_questions * sizeof(char *));
    if (!tree->questions)
    {
        logger_error("Failed to allocate memory for questions");
        free(unique_questions);
        return false;
    }

    for (int i = 0; i < num_unique_questions; i++)
    {
        tree->questions[i] = my_strdup(unique_questions[i]);
        if (!tree->questions[i])
        {
            logger_error("Failed to allocate memory for question");

            // Free already allocated questions
            for (int j = 0; j < i; j++)
            {
                free(tree->questions[j]);
            }
            free(tree->questions);
            tree->questions = NULL;
            free(unique_questions);
            return false;
        }
    }

    tree->num_questions = num_unique_questions;
    free(unique_questions);

    return true;
}

bool dicotomic_tree_validate(DicotomicTree *tree)
{
    if (!tree || tree->num_species == 0)
    {
        logger_error("Invalid tree or no species");
        return false;
    }

    // Extract questions if not already done
    if (tree->num_questions == 0)
    {
        if (!dicotomic_tree_extract_questions(tree))
        {
            return false;
        }
    }

    // Check that all species follow the same question order
    for (int i = 0; i < tree->num_species; i++)
    {
        Species *species = tree->species[i];

        if (!species_follows_question_order(species, (const char **)tree->questions, tree->num_questions))
        {
            logger_warning("Species '%s' does not follow the expected question order", species->name);
            // We don't return false here because the spec says to skip species with wrong order
        }
    }

    return true;
}

void dicotomic_tree_free(DicotomicTree *tree)
{
    if (!tree)
    {
        return;
    }

    // Free name
    free(tree->name);

    // Free species
    for (int i = 0; i < tree->num_species; i++)
    {
        species_free(tree->species[i]);
    }
    free(tree->species);

    // Free questions
    for (int i = 0; i < tree->num_questions; i++)
    {
        free(tree->questions[i]);
    }
    free(tree->questions);

    // Free tree
    free(tree);
}