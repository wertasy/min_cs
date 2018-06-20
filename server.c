#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUF_SIZE 1024

const int port = 8421;

int main(int argc, const char **argv) {
    fprintf(stdout, "this is server!\n");

    struct sockaddr_in server, client;

    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("socket err");
        exit(1);
    }

    memset((char *)&server, 0, sizeof(server));
    server.sin_family = AF_INET;        // 地址族:必须总是设置为 AF_INET
    server.sin_port = htons(port);      // 按网络字节次序的端口

    /* 将一个名字和一个套接字绑定到一起(给一个套接字赋一个本地变量名) */
    if (bind(server_sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind err");
        exit(1);
    }

    /**
     * listen
     * 创建一个连接队列，开始监听(自动接收到来的连接)并且
     * 为连接队列(已经完成连接正等待应用程序接收的套接字队
     * 列)指定一个长度限制
     */
    listen(server_sock, 5);

    /**
     * accept
     * 从未完成连接队列中取出第一个连接请求,创建一个和参数 s
     * 属性相同的连接套接字,并为这个套接字分配一个文件描述符,
     * 然后以这个描述符返回.
     */
    socklen_t n = (socklen_t)sizeof(client);
    int conn_sock;
    while ((conn_sock = accept(server_sock, (struct sockaddr *)&client, &n)) > 0) {
        unsigned char buf[BUF_SIZE + 1];
        memset(buf, 0, sizeof(buf));

        /* 从连接套接字接收数据 */
        if ((n = recv(conn_sock, buf, BUF_SIZE, 0)) < 0) {
            perror("recv err");
        } else {
            buf[n] = 0;
            fprintf(stdout, "recv from client : %s\n", buf);
        }

        /* 从连接套接字发送数据 */
        char *msg = "Hello client!";
        if (send(conn_sock, msg, strlen(msg), 0) < 0) {
            perror("send err");
        }
        close(conn_sock);
    }
    if (conn_sock < 0) perror("accept err");

    close(server_sock);
    exit(0);
}
