#include <chrono>
#include <cstdlib>
#include <iostream>
#include <pthread.h>


#define MAXTHREADS 8


std::chrono::_V2::system_clock::time_point start;

void *printmsg(void *msg)
{
	std::chrono::_V2::system_clock::time_point finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed = finish - start;
	std::cout << "Time to make a new pthread: " << elapsed.count() << " ms" << std::endl;

	long threadidx = (long) msg;
	std::cout << "Threadidx: " << threadidx << std::endl;
	pthread_exit(NULL);
}

int main()
{
	pthread_t threads[MAXTHREADS];

	for(uint32_t i = 0; i < MAXTHREADS; i++)
	{
		std::cout << std::endl;

		start			  = std::chrono::high_resolution_clock::now();
		int thread_create = pthread_create(&threads[i], nullptr, printmsg, (void *) i);

		if(thread_create)
		{
			std::cout << "Could not create thread " << i << std::endl;
			exit(-1);
		}
	}

	for(int i = 0; i < MAXTHREADS; i++)
	{
		pthread_join(threads[i], nullptr);
	}

	return 0;
}