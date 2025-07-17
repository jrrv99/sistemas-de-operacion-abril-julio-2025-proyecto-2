#include "create_directory_structure.h"
#include "../../../include/common/utils.h"
#include "../../../include/common/logger.h"
#include "../../infrastructure/process/process_manager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// Helper function to create the path for a characteristic
static char *create_characteristic_path(
    const char *root_dir,
    const char *question,
    bool answer,
    const char *true_text,
    const char *false_text,
    ConcatMode concat_mode)
{
    const char *text = answer ? true_text : false_text;
    char *path = NULL;

    switch (concat_mode)
    {
    case PREFIX_MODE:
        path = malloc(strlen(root_dir) + strlen(text) + strlen(question) + 3);
        if (path)
        {
            sprintf(path, "%s/%s %s", root_dir, text, question);
        }
        break;
    case SUFFIX_MODE:
        path = malloc(strlen(root_dir) + strlen(question) + strlen(text) + 3);
        if (path)
        {
            sprintf(path, "%s/%s %s", root_dir, question, text);
        }
        break;
    case BOTH_MODES:
        path = malloc(strlen(root_dir) + strlen(text) + strlen(question) + strlen(text) + 4);
        if (path)
        {
            sprintf(path, "%s/%s %s %s", root_dir, text, question, text);
        }
        break;
    }

    return path;
}

// Helper function to create directories for a species
static StatusCode create_species_directories(
    const Species *species,
    const char *root_dir,
    const char **questions,
    int num_questions,
    const DirectoryCreationConfig *config,
    const FileSystemPort *file_system)
{
    char *current_path = my_strdup(root_dir);
    if (!current_path)
    {
        return ERROR_MEMORY_ALLOCATION;
    }

    StatusCode error = SUCCESS;

    // Create directories for each characteristic
    for (int i = 0; i < num_questions; i++)
    {
        const char *question = questions[i];

        // Find the answer for this question
        bool found = false;
        bool answer = false;

        for (int j = 0; j < species->num_characteristics; j++)
        {
            if (strcmp(species->characteristics[j].question, question) == 0)
            {
                found = true;
                answer = species->characteristics[j].answer;
                break;
            }
        }

        if (!found)
        {
            // Skip questions that don't apply to this species
            continue;
        }

        // Create the path for this characteristic
        char *new_path = create_characteristic_path(
            current_path,
            question,
            answer,
            config->true_text,
            config->false_text,
            config->concat_mode);

        if (!new_path)
        {
            free(current_path);
            return ERROR_MEMORY_ALLOCATION;
        }

        // Create the directory if it doesn't exist
        if (!file_system->directory_exists(new_path))
        {
            error = file_system->create_directory(new_path);
            if (error != SUCCESS)
            {
                free(current_path);
                free(new_path);
                return error;
            }
        }

        // Update the current path
        free(current_path);
        current_path = new_path;
    }

    // Create the species file
    char *species_file_path = malloc(strlen(current_path) + strlen(species->name) + 6);
    if (!species_file_path)
    {
        free(current_path);
        return ERROR_MEMORY_ALLOCATION;
    }

    sprintf(species_file_path, "%s/%s.txt", current_path, species->name);

    // Create the file if it doesn't exist
    if (!file_system->file_exists(species_file_path))
    {
        error = file_system->create_file(species_file_path);
    }

    free(species_file_path);
    free(current_path);

    return error;
}

StatusCode create_directory_structure(
    const DicotomicTree *tree,
    const DirectoryCreationConfig *config,
    const FileSystemPort *file_system)
{
    // Create the root directory if it doesn't exist
    if (!file_system->directory_exists(config->root_dir))
    {
        StatusCode error = file_system->create_directory(config->root_dir);
        if (error != SUCCESS)
        {
            return error;
        }
    }

    // If using multiple processes, create a child process for each species
    if (config->use_multiple_processes)
    {
        int max_processes = get_max_processes();
        int active_processes = 0;

        for (int i = 0; i < tree->num_species; i++)
        {
            // Wait if we've reached the maximum number of processes
            if (active_processes >= max_processes)
            {
                wait(NULL);
                active_processes--;
            }

            // Create a child process
            pid_t pid = create_child_process();

            if (pid == 0)
            {
                // Child process
                StatusCode error = create_species_directories(
                    tree->species[i],
                    config->root_dir,
                    (const char **)tree->questions,
                    tree->num_questions,
                    config,
                    file_system);

                exit(error);
            }
            else if (pid > 0)
            {
                // Parent process
                active_processes++;
                logger_info("Created process %d for species %s", pid, tree->species[i]->name);
            }
            else
            {
                // Error creating process
                logger_error("Failed to create process for species %s", tree->species[i]->name);
                return ERROR_MEMORY_ALLOCATION;
            }
        }

        // Wait for all child processes to finish
        if (!wait_for_child_processes())
        {
            return ERROR_DIRECTORY_CREATION;
        }
    }
    else
    {
        // Create directories sequentially
        for (int i = 0; i < tree->num_species; i++)
        {
            StatusCode error = create_species_directories(
                tree->species[i],
                config->root_dir,
                (const char **)tree->questions,
                tree->num_questions,
                config,
                file_system);

            if (error != SUCCESS)
            {
                return error;
            }
        }
    }

    return SUCCESS;
}