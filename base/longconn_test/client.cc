#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <pthread.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int create_socket(char* server, int port)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sockfd) {
        printf("create socket failed, errno %d\n", errno);
        exit(1);
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(server);
    addr.sin_port = htons(port);
    if (-1 == connect(sockfd, (struct sockaddr *) &addr, sizeof(struct sockaddr))) {
        printf("connect failed, errno %d\n", errno);
        exit(1);
    }
    return sockfd;
};
const char *ip = "xxx.xxx.xx.xx";
const uint16_t port = 8606;

void* thread_1(void* p)  
{  
    int sock = create_socket(const_cast<char*>(ip), port);
    char s[5];
    uint64_t sum = 0;
    for (int i = 0; i < 1024; i++) {
        memcpy(s, (void*)&i, sizeof(i));
        s[4] = '\0';
        //printf("%d, %d\n", *(int*)s, strlen(s));
        send(sock, s, sizeof(i), 0);
        sum += i;
    }
    printf("%lu\n", sum);
    return NULL;
} 


int main()
{
    pthread_t id_1,id_2;  
    int i, ret; 
    ret = pthread_create(&id_1, NULL, thread_1, NULL);  
    if(ret != 0)  
    {  
        printf("Create pthread error!\n");  
        return -1;  
    }  
    ret = pthread_create(&id_2, NULL, thread_1, NULL);  
    if(ret !=0 )  
    {  
        printf("Create pthread error!\n");  
        return -1;  
    }  

    pthread_join(id_1, NULL);  
    pthread_join(id_2, NULL);  
    return 0;  

}
