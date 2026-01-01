#include "../headers/main.h"

void free_everything(int *fd, int *client_fd){
    close(*fd);
    close(*client_fd);
}

int act_as_a_client(char* host, char* request, int client_fd, int req_len){

    struct addrinfo hints={0}, *res, *i;

    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC;

    if(getaddrinfo(host, "80", &hints, &res) != 0) {
        fprintf( stderr,"error getting address info\n" );
        return -1;
    }

    int server_fd=0;

    for(i = res; i != NULL; i=i->ai_next) {
        server_fd= socket( i -> ai_family, i -> ai_socktype, i -> ai_protocol);
        if (server_fd==-1) continue;
        if (connect(server_fd, i->ai_addr, i->ai_addrlen)==0){
            break;
        }
        close(server_fd);
        server_fd=-1;   
    }

    freeaddrinfo(res);

    if(server_fd==-1){
        fprintf(stderr,"connection failed\n");
        return -1;
    }

    send(server_fd, request, req_len,0);

    char buf[4096];
    int n;

    while ((n = recv(server_fd, buf, sizeof(buf), 0)) > 0) {
        send(client_fd, buf, n, 0);
    }

    close(server_fd);
    return 0;

}


int get_host(char *host, char *buf, int size_of_host) {
    if (!buf || !host) return -1;

    char *p = strstr(buf, "Host:");
    if (!p) return -1;

    p += 5;                 
    while (*p == ' ') p++; 

    int i = 0;
    while (*p && *p != '\r' && *p != '\n' && i < size_of_host - 1) {
        host[i++] = *p++;
    }

    host[i] = '\0';
    return 0;
}
