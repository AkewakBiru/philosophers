#include <sys/time.h>
#include <stdio.h>

unsigned long	get_time()
{
	unsigned long tm;
	struct timeval	time;

	gettimeofday(&time, 0);
	// time in sec and microsecond converted to millisec
	tm = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (tm);
}
