#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <pthread.h>

sem_t *semaphore;

// sem_init && sem_destroy are deprecated
// if in anycase the semaphore's value is 0 and sem_wait is tried
// by a process, it shouldn't work (should block)
int main()
{
	pid_t proc;
	int i=0, j =0, k=0;
	// initialize semaphore
	// O_EXCL -> returns an error if a semaohore with the given
	// name already exists
	semaphore = sem_open("sem", O_CREAT | O_EXCL, 0644, 1);
	// if (semaphore == SEM_FAILED)
	// {
	// 	perror("semaphore");
	// 	exit(EXIT_FAILURE);
	// }
	for (j= 0; j < 2; j++)
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
			i++;
			printf("i = %d\n", i);
			sleep(1);
			// unlock
			sem_post(semaphore);
			exit(EXIT_SUCCESS);
		}
		// printf("inside parent i = %d\n", i);
	}
	k = -1;
	while (++k < j)
		waitpid(-1, 0, 0);
	// destroy semaphore
	sem_unlink("sem");
	// returns -1 on failure
	sem_close(semaphore);
	printf("inside parent i = %d\n", i);
}
