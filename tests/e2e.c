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
      
    
    bzero(&servaddr, sizeof(servaddr)); 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_family = AF_INET; 
      
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
      
    
    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) { 
        perror("Error: Connection Failed"); 
        exit(0); 
    } 
  
    
    int sent = send(sockfd, message, strlen(message), 0); 
    if (sent == -1) {
        perror("send failed");
        close(sockfd);
        return;
    }
    printf("Client sent %d bytes: %s\n", sent, message);
      
    
    n = recv(sockfd, buffer, sizeof(buffer) - 1, 0); 
    if (n == -1) {
        perror("recv failed");
        close(sockfd);
        return;
    }
    buffer[n] = '\0';
    printf("Client received %d bytes: %s\n", n, buffer); 
      
    
    close(sockfd);
}



void* client_thread(void* arg) {
    run_e2e();
    return NULL;
}

