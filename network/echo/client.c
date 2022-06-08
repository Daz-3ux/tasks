#include "head.h"

int main(int argc, char **argv)
{
    int clientFd;
    struct sockaddr_in serveraddr;
    char buf[MAXBUFFER];
    clientFd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(1234);
    inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr);
    connect(clientFd, (struct sockaddr *)&serveraddr,
    sizeof(serveraddr));
    while(1){
        bzero(buf, sizeof(buf));
        scanf("%s", buf);
        write(clientFd, buf, sizeof(buf));
    }

    close(clientFd);
    return 0;
}