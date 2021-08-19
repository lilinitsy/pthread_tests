#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <netinet/in.h>
#include <stdexcept>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define PORT 1234

struct Client
{
	int socket_fd;

	Client()
	{
		socket_fd = socket(AF_INET, SOCK_STREAM, 0);
		if(socket_fd == -1)
		{
			throw std::runtime_error("Could not create a socket");
		}
	}

	void connect_to_server(int port)
	{
		sockaddr_in server_address = {
			.sin_family = AF_INET,
			.sin_port	= htons(static_cast<in_port_t>(port)),
			//.sin_addr	= htonl(0xc0a80002),
		};


		int connect_result = connect(socket_fd, (sockaddr *) &server_address, sizeof(server_address));
		if(connect_result == -1)
		{
			throw std::runtime_error("Could not connect to server");
		}
	}
};

Client client;

void *receive_packets(void*)
{
	char servbuf[8];
	uint32_t num_bytes = 8;
	size_t servbufidx = 0;
	do
	{
		size_t server_read = read(client.socket_fd, &servbuf[servbufidx], num_bytes);
		servbufidx += server_read;
		send(client.socket_fd, "fu", 2, 0);
	} while (servbufidx < num_bytes);
	

	return (void*) servbuf;
}

void *average_inputnums(void *inputnums)
{
	long *numbers = (long*) inputnums;

	long avg = (numbers[0] + numbers[1] + numbers[2] + numbers[3]) / 4;

	return (void*) avg;
}

int main()
{
	pthread_t network_thread;
	pthread_t work_thread;
	client = Client();
	client.connect_to_server(PORT);

	long inputnums[4];

	for(uint32_t i = 0; i < 4; i++)
	{
		std::cin >> inputnums[i];
	}

	char servbuf[8];
	void *avgnum;

	int network_thread_create = pthread_create(&network_thread, nullptr, receive_packets, nullptr);
	std::cout << "Created network thread" << std::endl;
	int average_thread_Create = pthread_create(&work_thread, nullptr, average_inputnums, (void *) inputnums);
	std::cout << "Created numavg thread" << std::endl;

	pthread_join(network_thread, (void**) servbuf);
	std::cout << "Joined network thread" << std::endl;
	pthread_join(work_thread, &avgnum);
	std::cout << "Joined avgnum work thread" << std::endl;

	std::cout << "Avg: " << avgnum << std::endl;

	return 0;
}