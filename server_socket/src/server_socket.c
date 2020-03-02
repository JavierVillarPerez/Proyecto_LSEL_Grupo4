/*
 ============================================================================
 Name        : server_socket.c
 Author      : Javier Villar Pérez
 Version     :
 Copyright   : Your copyright notice
 Description : server socket for DEMO communication.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "ring_buf.h"

#define PORT 8080

int main(void) {

    int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	device_buf_t buffer[1024] = {0};
	char *hello = "Hello from server";

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
		printf("Waiting incoming socket\n");
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
						   (socklen_t*)&addrlen))<0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}
		printf("New socket arrived\n");

		valread = read( new_socket , buffer, 1024);

    	if( valread > 0)
    	{
			printf("-----------------\n");
			valread = 0;
			printf("Device received ID: %d\n",buffer[0].ID );
			printf("Device received sensor1 measure: %d\n",buffer[0].sensor1.measure);
			printf("Device received sensor2 measure: %d\n",buffer[0].sensor2.measure );
			printf("Device received sensor3 measure: %d\n",buffer[0].sensor3.measure );

			send(new_socket , hello , strlen(hello) , 0 );
			printf("Hello message sent\n");
    	}
    }
return 0;
}
