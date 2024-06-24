#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int sockfd, newsockfd, received_bytes = 0;
    struct sockaddr_in6 local_addr, remote_addr;
    socklen_t s_len = sizeof(struct sockaddr_in6);
    char msg[1000];

    if(argc != 2) {
        printf("Insert port number\n");
        return -1;
    }

    if((sockfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
        printf("Error while socket opening\n");
        return -1;
    }

    memset(&local_addr, 0, s_len);
    local_addr.sin6_family = AF_INET6;
    local_addr.sin6_port = htons(atoi(argv[1]));

    if(bind(sockfd, (struct sockaddr *)&local_addr, s_len) < 0) {
        printf("Error while binding\n");
        return -1;
    }
    
    listen(sockfd, 5);

    for(;;) {
        newsockfd = accept(sockfd, (struct sockaddr *)&remote_addr, &s_len);

        if(fork() == 0) {
            close(sockfd);

            for(;;) {
                received_bytes = recv(newsockfd, msg, 999, 0);
                msg[received_bytes] = 0;
                printf("PORT = %d, msg = %s\n",ntohs(remote_addr.sin6_port), msg);

                send(newsockfd, msg, received_bytes, 0);

                if(strcmp(msg, "end\n") == 0) {
                    printf("End of communication\n");
                    return -1;
                }
            }
        } else {
            close(newsockfd);
        }
    }

    close(sockfd);

    return 0;
}