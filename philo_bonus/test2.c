#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <pthread.h>
# define COL_FORK "\033[0;32m"
# define COL_EAT "\033[0;33m"
# define COL_THINK "\033[0;35m"
# define COL_SLEEP "\033[0;36m"
# define COL_DFL "\033[0;37m"

sem_t *semaphore;

sem_t *sem_print;

unsigned long	get_time(void)
{
	unsigned long	tm;
	struct timeval	time;

	gettimeofday(&time, 0);
	tm = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (tm);
}

int	wait_action(unsigned long start, unsigned long time, unsigned long last_ate, unsigned long time_to_die)
{
	while (get_time() - start < time)
	{
		if (get_time() - last_ate >= time_to_die)
			return (0);
		usleep(200);
	}
	return (1);
}

void routine(int eat_time, int sleep_time, int die_time, unsigned long start_time, int id)
{
	unsigned long last_ate = get_time();
	start_time = get_time();
	sem_t *sema = sem_open("sem", O_CREAT | O_EXCL, 2);
		// printf("here\n");
	sem_t *printer = sem_open("sem_print", O_CREAT | O_EXCL, 2);
	while (get_time() - last_ate < die_time)
	{
		sem_wait(sema);
		sem_wait(printer);
		printf("%s[%lu] %d has taken a fork\n%s", COL_FORK, get_time() - start_time, id, COL_DFL);
		sem_post(printer);
		sem_wait(sema);
		sem_wait(printer);
		printf("%s[%lu] %d has taken a fork\n%s", COL_FORK, get_time() - start_time, id, COL_DFL);
		sem_post(printer);
		sem_wait(printer);
		printf("%s[%lu] %d is eating\n%s", COL_EAT, get_time() - start_time, id, COL_DFL);
		sem_post(printer);
		last_ate = get_time();
		if (!wait_action(get_time(), eat_time, last_ate, die_time))
			exit(id);
		sem_post(sema);
		sem_post(sema);
		sem_wait(printer);
		printf("%s[%lu] %d is sleeping\n%s", COL_SLEEP, get_time() - start_time, id, COL_DFL);
		sem_post(printer);
		if (!wait_action(get_time(), sleep_time, last_ate, die_time))
			exit(id);
	}
	exit(EXIT_SUCCESS);
}

int main()
{
	semaphore = sem_open("sem", O_CREAT | O_EXCL, 2);
	sem_print = sem_open("sem_print", O_CREAT | O_EXCL, 1);
	pid_t proc;
	int	eat_time = 200;
	int sleep_time = 200;
	int die_time = 210;
	int	e_status = 0;
	unsigned long start_time = get_time();

	int i = -1;
	while (++i < 2)
	{
		proc = fork();
		if (proc == -1)
			perror("FOrk");
		if (proc == 0)
		{
			routine(eat_time, sleep_time, die_time, start_time, i);
		}
	}
	waitpid(-1, &e_status, 0);
	if (WIFEXITED(e_status))
		e_status = WEXITSTATUS(e_status);
	sem_close(semaphore);
	sem_unlink("sem");
	sem_close(sem_print);
	sem_unlink("sem_print");
	printf("%d died\n", e_status);
}
