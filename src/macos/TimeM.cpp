/*
 * TimeM.cpp - Time measurement  (macOS version)
 */

#include "TimeM.h"


TimeM::TimeM()
{
	t_start.tv_sec = 0;
	t_start.tv_nsec = 0;
}


unsigned long int TimeM::start()
{
	unsigned int t_el = elapsed();
	clock_gettime(CLOCK_REALTIME, &t_start);
	return t_el;
}


unsigned long int TimeM::elapsed()
{
	struct timespec t;
	if (!t_start.tv_sec && !t_start.tv_nsec) return 0;
	unsigned long int delta_s=0, delta_ns=0, t_el=0;
	clock_gettime(CLOCK_REALTIME, &t);
	delta_s = t.tv_sec - t_start.tv_sec;
	if (t.tv_nsec >= t_start.tv_nsec)
		delta_ns = t.tv_nsec - t_start.tv_nsec;
	else
	{
		delta_ns = 1000000000 - t_start.tv_nsec + t.tv_nsec;
		delta_s--;
	}
	t_el = delta_s*1000 + delta_ns/1000000;
	return t_el;
}
