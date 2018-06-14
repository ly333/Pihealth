/*************************************************************************
	> File Name: socket_create_listen.h
	> Author: luoyi
	> Mail: 596890983@qq.com
	> Created Time: 2018年03月12日 星期一 11时38分07秒
 ************************************************************************/

#ifndef _SOCKET_CREATE_LISTEN_H
#define _SOCKET_CREATE_LISTEN_H

#include "head.h"
int socket_create(int port) {
        int sockfd, yes;
        if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
                    perror("socket() error\n");
                    return -1;
                }
        if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
                    perror("setsockopt() error\n");
                    close(sockfd);
                    return -1;
                }
        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 ) {
            printf("port is %d\n", port);
                    perror("bind() error\n");
                    close(sockfd);
                    return -1;
                }
        if(listen(sockfd, 20) < 0) {
                    perror("listen() error\n");
                    close(sockfd);
                    return -1;
                }
        return sockfd;
}

void get_conf_value(const char *filename, const char *str, char *buffer) {
        FILE *fin = fopen(filename, "r");
        char *filecontent = (char *)malloc(sizeof(char) * 1000);
        while(fscanf(fin, "%s", filecontent) != EOF) {
                    int i;
                    for(i = 0; str[i]; i++) {
                                    if(str[i] != filecontent[i]) break;
                                }
                    if(!str[i]) {
                                    strcpy(buffer, filecontent + i + 1);
				    return ;
                                }
                }
        return ;
}

int send_file(const char *filename, int sockfd_control);

int react_client(int sockfd, char *str) {
    char *buffer = (char *)malloc(sizeof(char) * 1000);
    if(strcmp(str, "100") == 0) {
        strcpy(buffer, "200");
        send(sockfd, buffer, 1000, 0);
        printf("send 200 to server\n");
        if(send_file("cpu.log", sockfd) == -1) {
            perror("send file error\n");
            return -1;
        }
    } else if(strcmp(str, "101") == 0) {
        strcpy(buffer, "201");
        send(sockfd, buffer, 1000, 0);
        printf("send 201 to server\n");
        if(send_file("disk.log", sockfd) == -1) {
            perror("send file error\n");
            return -1;
        }
    } else if(strcmp(buffer, "102") == 0) {
        strcpy(buffer, "202");
        send(sockfd, buffer, 1000, 0);
        printf("send 202 to server\n");
        if(send_file("mem.log", sockfd)) {
            perror("send file error\n");
            return -1;
        }
    } else {
        perror("no file");
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer, "404");
        send(sockfd, buffer, 1000, 0);      
        printf("send 404 to server\n");
    }
    return 0;
}

int send_file(const char *filename, int sockfd_control) {
    int client_listen, port, sockfd;
    char port_str[20];
    //对文件加锁
    FILE *fin = fopen(filename, "r");
    if(flock(fileno(fin), LOCK_EX) == -1) {
        perror("lock file error\n");
        return -1;
    }
    //建立数据连接的监听
    get_conf_value("file", "client_port2", port_str);
    port = atoi(port_str);
    if((client_listen = socket_create(port)) < 0) {
        perror("socket_create1() error\n");
        return -1;
    }
    //发送ACK信号
    char *buffer = (char *)malloc(sizeof(char) * 1000);
    strcpy(buffer, "ACK1");
    send(sockfd_control, buffer, 1000, 0);
    printf("send ACK to server\n");
    memset(buffer, 0, sizeof(buffer));
    //建立数据连接
    struct sockaddr_in server_addr;
    socklen_t len = sizeof(server_addr);
    sleep(5);
    if((sockfd = accept(client_listen, (struct sockaddr *)&server_addr, &len)) < 0) {
        perror("accept error\n");
        return -1;                  
    }
    //发送文件
    printf("filename is %s\n", filename);
    while(fgets(buffer, 1000, fin) != NULL) {
        send(sockfd, buffer, 1000, 0);
        printf("send %s\n", buffer);
        memset(buffer, 0, sizeof(buffer));                  
    }
    //对文件解锁
    if(flock(fileno(fin), LOCK_UN) == -1) {
        perror("unlock error\n");
        return -1;
    }
    fclose(fin);
    fin = fopen(filename, "w");
    fclose(fin);
    close(client_listen);
    close(sockfd);
    return 0;
}

void Monitor(const char *command, const char *filename) {
    while(1) {
        char *buffer = (char *)malloc(sizeof(char) * 10000);
        char *str = (char *)malloc(sizeof(char) * 1000);
        FILE *fp = NULL;
        for(int i = 1; i <= 12; i++) {
            fp = popen(command, "r");
            fscanf(fp, "%s", str);
            strcat(buffer, str);
            memset(str, 0, sizeof(str));
            sleep(5);        
        }
        fp = fopen(filename, "a+");
        fprintf(fp, "%s", buffer);
        memset(buffer, 0, sizeof(buffer));    
    }
}


#endif
