#include <cstdlib>
#include <iostream>
#include <pthread.h>


#define MAXTHREADS 8

void *printmsg(void *msg)
{
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