#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUF_SIZE 1024

const char *ip = "127.0.0.1";
const int port = 8421;

int main(int argc, const char **argv) {
    fprintf(stdout, "this is client!\n");
    fprintf(stdout, "conn server ip is %s, port is %d\n", ip, port);

    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket err");
        exit(1);
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    struct hostent *hp;
    hp = gethostbyname(ip);
    memcpy((char *)&servaddr.sin_addr, (char *)hp->h_addr, hp->h_length);

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect err");
        exit(1);
    }

    char msg[BUF_SIZE] = "Hello Server!";
    if (send(sockfd, msg, strlen(msg), 0) < 0) {
        perror("send err");
        exit(1);
    }

    int n;
    char buf[BUF_SIZE + 1];
    while ((n = recv(sockfd, buf, BUF_SIZE, 0)) > 0) {
        buf[n] = 0;
        fprintf(stdout, "recv from server : %s\n", buf);
    }
    if (n < 0) perror("recv err");

    close(sockfd);

    exit(0);
}
