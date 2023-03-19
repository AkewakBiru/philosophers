// #include "philo.h"
#include <sys/time.h>
#include <stdio.h>

long long int	get_time()
{
	long long int tm;
	struct timeval	time;

	gettimeofday(&time, 0);
	// changes time to milliseconds (doesn't change anything, but i can use whatever start time i want)
	tm = time.tv_sec;
	return (tm);
}

// void get_time_diff(long long t1, long long t2)
// {
	
// }

// int main()
// {
// 	long long time;

// 	time = get_time();
// 	// time = time * 1000;
// 	printf("%lld\n", time);
// 	// get_time_diff();
// 	// get_time_diff();
// }
