#include "process_manager.h"
#include "../../../include/common/logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/sysinfo.h>

int create_child_process(void)
{
    return fork();
}

bool wait_for_child_processes(void)
{
    int status;
    pid_t pid;
    bool success = true;

    while ((pid = wait(&status)) > 0)
    {
        if (WIFEXITED(status))
        {
            int exit_status = WEXITSTATUS(status);
            if (exit_status != 0)
            {
                logger_error("Process %d exited with status %d", pid, exit_status);
                success = false;
            }
            else
            {
                logger_info("Process %d completed successfully", pid);
            }
        }
        else if (WIFSIGNALED(status))
        {
            logger_error("Process %d killed by signal %d", pid, WTERMSIG(status));
            success = false;
        }
    }

    return success;
}

int get_max_processes(void)
{
    // Get number of available processors
    int num_processors = sysconf(_SC_NPROCESSORS_ONLN);

    // Use at most num_processors processes
    return (num_processors > 0) ? num_processors : 1;
}