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


struct Server
{
	int socket_fd;
	int client_fd;

	Server()
	{
		// Create socket
		socket_fd = socket(AF_INET, SOCK_STREAM, 0);
		if(socket_fd == 0)
		{
			throw std::runtime_error("Socket creation failed");
		}
	}

	void connect_to_client(int port)
	{

		// define the address struct to be for TCP using this port
		sockaddr_in address = {
			.sin_family = AF_INET,
			.sin_port	= static_cast<in_port_t>(htons(port)),
		};

		// bind to socket
		int binding = bind(socket_fd, (sockaddr *) &address, sizeof(address));
		if(binding == -1)
		{
			throw std::runtime_error("Bind to socket failed");
		}

		// Listen for a client to connect
		listen(socket_fd, 1);
		// Accept a connection from a client
		client_fd = accept(socket_fd, nullptr, nullptr);
	}
};

int main()
{
	Server server = Server();
	server.connect_to_client(PORT);

	const char *bufdata = "12345678";
	size_t numbytes = 8;
	send(server.client_fd, bufdata, numbytes, 0);

	size_t recidx = 0;
	char recbuf[2];
	do
	{
		size_t client_read = read(server.client_fd, &recbuf[recidx], 2);
		recidx += client_read;
	} while(recidx < 2);
	

	return 0;
}