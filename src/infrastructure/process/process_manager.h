#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <stdbool.h>

/**
 * @brief Create a child process
 *
 * @return int The process ID or -1 if creation failed
 */
int create_child_process(void);

/**
 * @brief Wait for all child processes to finish
 *
 * @return bool true if all processes finished successfully, false otherwise
 */
bool wait_for_child_processes(void);

/**
 * @brief Get the maximum number of processes that can be created
 *
 * @return int The maximum number of processes
 */
int get_max_processes(void);

#endif /* PROCESS_MANAGER_H */