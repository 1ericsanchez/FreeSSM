/*
 * TimeM.h - Time measurement  (macOS version)
 */

#ifndef TIMEM_H
#define TIMEM_H


#include <ctime>


class TimeM
{
public:
	TimeM();
	unsigned long int start();
	unsigned long int elapsed();
private:
	struct timespec t_start;
};


#endif
