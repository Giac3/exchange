#include <stdio.h> 
#include <strings.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include<netinet/in.h> 
#include <stdbool.h>
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



void* handle_order (struct socketaddr_in *cliaddr) {
    puts("Made it here");
    char *message = "Hello Client";
    sendto(listenfd, message, MAXLINE, 0, 
        (struct sockaddr*)&cliaddr, sizeof(cliaddr));
}

void poll_requests() {

    while (true) {

        struct sockaddr_in cliaddr;
        
        char buffer[100];
        int len = sizeof(cliaddr); 
        int n = recvfrom(listenfd, buffer, sizeof(buffer), 
                0, (struct sockaddr*)&cliaddr,&len);
        
        buffer[n] = '\0'; 

        if(pthread_create(NULL, NULL, handle_order, cliaddr) != 0) {
            perror("Failed to create thread");
        }


    }
}
  
// Driver code 
int main() {    

    listenfd = create_server();
    poll_requests();
} 
