#include <stdio.h>
#include "./e2e.h"
#include <pthread.h>
int main () {
    pthread_t threads[10];
    for(int i = 0; i < 10; i++) {
        if(pthread_create(&threads[i], NULL, client_thread, NULL) != 0) {
            perror("Failed to create thread");
        }
    }
    for(int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}