#pragma once
 
#include <netinet/in.h> 

typedef enum order_type {
    BUY,
    SELL,
    CANCEL,
} OrderType;
 

typedef struct order {
    struct sockaddr_in cliaddr;
    char buffer[100];
    int n;
} Order;

void add_order_to_book (void* order, struct sockaddr *cliaddr);
void match_order (void* order, struct sockaddr *cliaddr);