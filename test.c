#include <stdio.h>
#include <pthread.h>

int j = 0;
pthread_mutex_t lock;

void *routine()
{
	printf("here\n");
	pthread_mutex_lock(&lock);
	for (int i = 0; i< 20000; i++)
		j++;
	pthread_mutex_unlock(&lock);
	return (0);
}

int main()
{
	pthread_t t_id1;
	pthread_t t_id2;

	pthread_mutex_init(&lock, 0);
	pthread_create(&t_id1, 0, routine, 0);
	pthread_create(&t_id2, 0, routine, 0);

	printf("in parent scope\n");
	pthread_join(t_id1, 0);
	pthread_join(t_id2, 0);

	pthread_mutex_destroy(&lock);
	printf("%d\n", j);
}