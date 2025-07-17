#ifndef DICOTOMIC_TREE_H
#define DICOTOMIC_TREE_H

#include "species.h"
#include "../../../include/common/types.h"

/**
 * @brief Represents a dicotomic tree
 */
typedef struct
{
    char *name;
    Species **species;
    int num_species;
    char **questions;
    int num_questions;
} DicotomicTree;

/**
 * @brief Create a new dicotomic tree
 *
 * @param name The name of the tree
 * @return DicotomicTree* The created tree or NULL if memory allocation failed
 */
DicotomicTree *dicotomic_tree_create(const char *name);

/**
 * @brief Add a species to the tree
 *
 * @param tree The tree
 * @param species The species to add
 * @return bool true if successful, false otherwise
 */
bool dicotomic_tree_add_species(DicotomicTree *tree, Species *species);

/**
 * @brief Extract all unique questions from the species in the tree
 *
 * @param tree The tree
 * @return bool true if successful, false otherwise
 */
bool dicotomic_tree_extract_questions(DicotomicTree *tree);

/**
 * @brief Validate that all species follow the same question order
 *
 * @param tree The tree
 * @return bool true if all species follow the same order, false otherwise
 */
bool dicotomic_tree_validate(DicotomicTree *tree);

/**
 * @brief Free the memory allocated for a dicotomic tree
 *
 * @param tree The tree to free
 */
void dicotomic_tree_free(DicotomicTree *tree);

#endif /* DICOTOMIC_TREE_H */