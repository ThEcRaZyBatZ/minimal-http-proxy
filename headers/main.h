#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h>
#include <netinet/in.h>

#include <arpa/inet.h>


#define BACKLOGS 5

int act_as_a_client(char* host, char* request, int client_fd, int req_len);
int get_host(char *host, char *buf, int size_of_host);

void free_everything(int *fd, int *client_fd);

#endif