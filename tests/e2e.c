#include <stdio.h> 
#include <strings.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include "./e2e.h"
  
#define PORT 5000 
#define MAXLINE 1000 

void run_e2e() {
    char buffer[100]; 
    char *message = "Hello Server"; 
    int sockfd, n; 
    struct sockaddr_in servaddr; 
      
    // clear servaddr 
    bzero(&servaddr, sizeof(servaddr)); 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_family = AF_INET; 
      
    // create datagram socket 
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
      
    // connect to server 
    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) { 
        puts("Error: Connection Failed"); 
        exit(0); 
    } 
  
    // send datagram 
    send(sockfd, message, MAXLINE, 0); 
      
    // waiting for response 
    n = recv(sockfd, buffer, sizeof(buffer), 0); 
    buffer[n] = '\0'; // Null-terminate the received data
    puts(buffer); 
  
    // close the descriptor 
    close(sockfd);
}

// Thread function that calls run_e2e()
void* client_thread(void* arg) {
    run_e2e();
    return NULL;
}

