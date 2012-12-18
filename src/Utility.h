#ifndef _UTILITY_H
#define _UTILITY_H

// Includes
#include <inttypes.h>
#include <stddef.h>
#include <sys/time.h>
#define MILLION 1000000

//! Stopwatch to time events in a program.

class StopWatch
{
public:

	//! \brief  Default constructor.

	StopWatch()
	{
		start();
	}

	//! \brief  Start the stopwatch.

	void start(void)
	{
		struct timeval now;
		gettimeofday(&now, NULL);
		_startTime = (now.tv_sec * MILLION) + now.tv_usec;
	}

	//! \brief  Stop the stopwatch.
	//! \return Number of seconds since stopwatch was started.

	double stop(void)
	{
		struct timeval now;
		gettimeofday(&now, NULL);
		double elapsedTime = (now.tv_sec * MILLION) + now.tv_usec - _startTime;
		return (elapsedTime / (double)MILLION);
	}

private:

	//! Start time in microseconds.
	uint64_t _startTime;

};

template <typename T>
inline
T begin_block_range(T num_elements, T block_id, T num_blocks)
{
	return static_cast<T>((static_cast<double>(num_elements) / num_blocks) * block_id);
}

template <typename T>
inline
T end_block_range(T num_elements, T block_id, T num_blocks)
{
	return (block_id + 1 < num_blocks) ?
         static_cast<T>((static_cast<double>(num_elements) / num_blocks) * (block_id + 1)) :
         num_elements;
}

#endif
