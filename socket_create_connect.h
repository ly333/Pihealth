#include "head.h"
#include <stdio.h>

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

int short_connect(int port, char *addr, int i);

int long_connect(int port, char *addr, char *string, int i) {
    int sockfd, pid;
    if((sockfd = sock_create(port, addr)) < 0) {
	    printf("sock_create() error\n");
		return -1;
	}
	printf("Connect with client success for control!\n");
    char *buffer = (char *)malloc(sizeof(char) * 1000);
    char *react = (char *)malloc(sizeof(char) * 1000);
    int j;
    for(j = 0; j < 3; j++) {
        pid = fork();
        if(pid == 0) break;
    }
    if(j == 0) {
        //服务器不断向客户端发送请求
        while(1) {
            //发送请求信号
            strcpy(buffer, "100");
		    send(sockfd, buffer, 1000, 0);
            printf("send %s to client\n", buffer);
            memset(buffer, 0, sizeof(buffer));
            //接收反馈信息
		    recv(sockfd, buffer, 1000, 0);
            //不存在文件
		    if(strcmp(buffer, "404") == 0) {
		        printf("%s\n", buffer);
		        continue;
		    } else {
                //存在文件，获得数据连接的所需信息
                char addr[20], port_str[20];
                int port;
		        get_conf_value("file", string, addr);
	            get_conf_value("file", "client_port2", port_str);
	            port = atoi(port_str);
                printf("recv %s from client\n", buffer);
                memset(buffer, 0, sizeof(buffer));
                //接收到ACK信号
                recv(sockfd, buffer, 1000, 0);
                if(strcmp(buffer, "ACK1") == 0) {
                    printf("recv ACK1 from client\n");
                    //建立短连接
                    if(short_connect(port, addr, i) == -1) {
                        perror("short_connect() error\n");
                        return -1;
                    }
                }
		    }
	    }
        printf("close with client\n");
	    close(sockfd);
    }else if(j == 1) {
        //服务器不断向客户端发送请求
        while(1) {
            //发送请求信号
            strcpy(buffer, "101");
		    send(sockfd, buffer, 1000, 0);
            printf("send %s to client\n", buffer);
            memset(buffer, 0, sizeof(buffer));
            //接收反馈信息
		    recv(sockfd, buffer, 1000, 0);
            //不存在文件
		    if(strcmp(buffer, "404") == 0) {
		        printf("%s\n", buffer);
		        continue;
		    } else {
                //存在文件，获得数据连接的所需信息
                char addr[20], port_str[20];
                int port;
		        get_conf_value("file", string, addr);
	            get_conf_value("file", "client_port2", port_str);
	            port = atoi(port_str);
                printf("recv %s from client\n", buffer);
                memset(buffer, 0, sizeof(buffer));
                //接收到ACK信号
                recv(sockfd, buffer, 1000, 0);
                if(strcmp(buffer, "ACK1") == 0) {
                    printf("recv ACK1 from client\n");
                    //建立短连接
                    if(short_connect(port, addr, i) == -1) {
                        perror("short_connect() error\n");
                        return -1;
                    }
                }
		    }
	    }
        printf("close with client\n");
	    close(sockfd);
    } else {
        //服务器不断向客户端发送请求
        while(1) {
            //发送请求信号
            strcpy(buffer, "102");
		    send(sockfd, buffer, 1000, 0);
            printf("send %s to client\n", buffer);
            memset(buffer, 0, sizeof(buffer));
            //接收反馈信息
		    recv(sockfd, buffer, 1000, 0);
            //不存在文件
		    if(strcmp(buffer, "404") == 0) {
		        printf("%s\n", buffer);
		        continue;
		    } else {
                //存在文件，获得数据连接的所需信息
                char addr[20], port_str[20];
                int port;
		        get_conf_value("file", string, addr);
	            get_conf_value("file", "client_port2", port_str);
	            port = atoi(port_str);
                printf("recv %s from client\n", buffer);
                memset(buffer, 0, sizeof(buffer));
                //接收到ACK信号
                recv(sockfd, buffer, 1000, 0);
                if(strcmp(buffer, "ACK1") == 0) {
                    printf("recv ACK1 from client\n");
                    //建立短连接
                    if(short_connect(port, addr, i) == -1) {
                        perror("short_connect() error\n");
                        return -1;
                    }
                }
		    }
	    }
        printf("close with client\n");
	    close(sockfd);
    }
    return 0;
}		    
  
int short_connect(int port, char *addr, int i) {
    int sockfd;
    printf("hello\n");
    if((sockfd = sock_create(port, addr)) < 0) {
		printf("sock_create() error\n");
		return -1;
	}
    printf("Connect with client for data success\n");
    char *buffer = (char *)malloc(sizeof(char) * 1000);
    buffer[0] = i + '0';
    char *path = (char *)malloc(sizeof(char) * 100);
    get_conf_value("file", buffer, path);
    memset(buffer, 0, sizeof(char));
    FILE *file = fopen(path, "a+");
	while(recv(sockfd, buffer, 1000, 0) > 0) {
		/*fprintf(file, "%s", buffer);*/
        printf("%s\n", buffer);
		memset(buffer, 0, sizeof(buffer));
	}
	close(sockfd);
    return 0;
}


