#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <pthread.h>

sem_t *semaphore;

// sem_init && sem_destroy are deprecated
int main()
{
	pid_t proc;
	// initialize semaphore
	semaphore = sem_open("sem", O_CREAT | O_EXCL, 0644, 1);
	for (int i = 0; i < 2; i++)
	{
		proc = fork();
		if (proc < 0)
		{
			perror("FORK");
			sem_unlink("sem");
			sem_close(semaphore);
		}
		if (proc == 0)
		{
			// lock
			sem_wait(semaphore);
			printf("Inside the critical section of a child process\n");
			sleep(1);
			// unlock
			sem_post(semaphore);
			exit(EXIT_SUCCESS);
		}
	}
	waitpid(-1, 0, 0);
	// destroy semaphore
	sem_unlink("sem");
	sem_close(semaphore);
	// printf("inside parent");
}