#include "../headers/main.h"
//proxy model is getaddrinfo -> socket -> bind -> listen -> accept -> parse -> use port 80 as client -> connect -> close -> free


int main(){

    struct addrinfo hints={0}, *res, *i;

    hints.ai_flags    = AI_PASSIVE; //connects to localhost
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    
    if(getaddrinfo(NULL, "6969", &hints, &res) != 0){
        fprintf( stderr,"error getting address info\n" );
        return -1;
    }
 
    int fd=0;

    for(i=res;i!=NULL;i=i->ai_next){ //checks all combinations until it finds one from the linked list

        fd = socket(i->ai_family, i->ai_socktype, i->ai_protocol); //servers file descriptor 

        if(fd ==-1) continue; 
        if(bind(fd,i->ai_addr,i->ai_addrlen)==-1){ //binds to the fd
            close(fd);
            continue;
        }
        
        break;
    }

    if(i==NULL){ //either the fd cant be assigned or binding issue, check
        fprintf( stderr,"error binding\n" );
        return -1;
    }

    if(listen(fd, BACKLOGS)==-1){ //starts listening to requests, polls if multiple requests are sent
        fprintf( stderr,"error listening\n" );
        return -1;
    }

    freeaddrinfo(res); //frees the linkedlist of structs

    struct sockaddr_storage their_ip; //stores the clients ip details, not used here but idk why i kept it
    socklen_t their_ip_size = sizeof(their_ip);

    int client_fd, bytes;
    char buf[4096],host[512]={0};


    while(1){

        client_fd=accept(fd, (struct sockaddr*) &their_ip, &their_ip_size); //accepts the clients req and assigns an fd

        if(client_fd == -1){
            fprintf( stderr,"error accepting\n" );
            return -1;
        }

        bytes=recv(client_fd, buf, sizeof(buf)-1,0); //get the req from localhost

        if(bytes<=0){
            free_everything(&fd,&client_fd);
            return -1;
        }

        buf[bytes]='\0';
        printf("Request: \n%s\n",buf);

        
        if(get_host(host, buf, sizeof(host))==-1){ //gets host i.e "google.com" or something like that and puts it in host
            free_everything(&fd,&client_fd); 
            return -1;
        }

        if(act_as_a_client(host, buf, client_fd, bytes) == -1){ //sends google.com along with the client_fd and req
            free_everything(&fd,&client_fd); 
            return -1;
        }
        close(client_fd);
    }

    close(fd);
    return 0;

}