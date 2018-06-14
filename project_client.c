/*************************************************************************
	> File Name: my_client.c
	> Author: luoyi
	> Mail: 596890983@qq.com
	> Created Time: 2018年03月05日 星期一 21时37分36秒
 ************************************************************************/


#include<stdio.h>
#include "head.h"

int sock_create(int port, char *addr) {
    int sockfd;
    struct sockaddr_in server_addr;
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        /*创建客户端套接字*/
        perror("socket() error\n");
        return -1;
    }
    /*创建服务器套接字*/
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(addr);
    if(connect(sockfd, (struct sockaddr *)&server_addr,     sizeof(server_addr)) < 0) {/*将本地套接字与客户端套接字相连*/
        perror("connect() error\n");
        close(sockfd);
        return -1;
    }
    return sockfd;
}


int main() {
    int sockfd, port, op;
    char addr[20];
    scanf("%d %s", &port, addr);
    char *buffer = (char *)malloc(sizeof(char) * 1000);
    scanf("%d", &op);
    switch(op) {
        case 0:{ /*短连接，用于传字符串*/
            while(~scanf("%s", buffer)) {
            /*建连函数在while循环里面，每次传送一条语句就会建立一次连接*/
                if((sockfd = sock_create(port, addr)) < 0) {
                    printf("sock_create() error\n");
                    return -1;
                }
                send(sockfd, buffer, 1000, 0);/*发送语句*/
                memset(buffer, 0, 1000);/*清空发送文本内容*/
                close(sockfd);/*断开连接*/
                }
        }break;
        case 1: {/*长连接，用于发送文件和三个进程同步发*/
            int i;
            for (i = 0; i < 3; i++) {
                /*三次循环，创建三个子进程*/
                int pid = fork();
                if (pid == 0) break;
            }
            if (i == 0) {/*进程1*/
                while(1){
                /*一旦进程创建连接之后就会进入死循环，变成守护进程，
                    * 即尽管父进程关闭了*/
                    if((sockfd = sock_create(port, addr)) < 0) {
                        printf("sock_create() error\n");
                        return -1;
                    }
                    FILE *fin1 = popen("ps", "r");
                    while(fgets(buffer, 1000, fin1) != NULL) {
                        send(sockfd, buffer, 1000, 0);
                        memset(buffer, 0, 1000);
                    }
                    close(sockfd);
                    sleep(5);/*等待5*/
                }
            } else if (i == 1) {/*进程2*/
                while(1) {
                    if((sockfd = sock_create(port, addr)) < 0) {
                        printf("sock_create() error\n");
                        return -1;
                    }
                    FILE *fin2 = popen("echo $HOME", "r");
                    while(fgets(buffer, 1000, fin2) != NULL) {
                        send(sockfd, buffer, 1000, 0);
                        memset(buffer, 0, 1000);
                    }
                    close(sockfd);
                    sleep(5);
                }
            } else if (i == 2) {/*进程3*/
                while(1) {
                    if((sockfd = sock_create(port, addr)) < 0) {
                        printf("sock_create() error\n");
                        return -1;
                    }
                    FILE *fin3 = popen("ls -al", "r");
                    while(fgets(buffer, 1000, fin3) != NULL) {
                    send(sockfd, buffer, 1000, 0);
                    memset(buffer, 0, 1000);
                    }
                    close(sockfd);
                    sleep(5);
                }
            }
            close(sockfd);/*父进程关闭套接字*/
            exit(1);/*父进程关闭*/
        }break;
    }
    return 0;
}
