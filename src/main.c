#include <stdio.h>
#include <strings.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>

#define PORT 5000 
#define MAXLINE 1000 


static int listenfd;

int create_server() { 
    int listenfd; 
    struct sockaddr_in servaddr; 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // Create a UDP Socket 
    listenfd = socket(AF_INET, SOCK_DGRAM, 0);         
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_family = AF_INET;  
   
    // bind server address to socket descriptor 
    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 

    return listenfd;
}



void* handle_order (void *arg) {
    struct sockaddr_in *cliaddr = (struct sockaddr_in *)arg;
    char *message = "Hello Client";

    ssize_t sent = sendto(listenfd, message, strlen(message), 0, 
        (struct sockaddr*)cliaddr, sizeof(struct sockaddr_in));

    if (sent == -1) {
        perror("sendto failed");
    } else {
        printf("Server sent %zd bytes to client\n", sent);
    }

    free(cliaddr);
    return NULL;
}

void poll_requests() {
    while (true) {
        struct sockaddr_in cliaddr;
        char buffer[100];
        socklen_t len = sizeof(cliaddr); 
        int n = recvfrom(listenfd, buffer, sizeof(buffer) - 1, 
                0, (struct sockaddr*)&cliaddr, &len);
        
        if (n == -1) {
            perror("recvfrom failed");
            continue;
        }
        buffer[n] = '\0'; 
        printf("Server received %d bytes: %s\n", n, buffer);

        pthread_t thread;
        struct sockaddr_in *cliaddr_ptr = malloc(sizeof(struct sockaddr_in));
        memcpy(cliaddr_ptr, &cliaddr, sizeof(struct sockaddr_in));

        if(pthread_create(&thread, NULL, handle_order, cliaddr_ptr) != 0) {
            perror("Failed to create thread");
            free(cliaddr_ptr);
        }

        
    }
}
  

int main() {    

    listenfd = create_server();
    poll_requests();
} 
