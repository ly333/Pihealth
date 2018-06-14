/*************************************************************************
	> File Name: my_server.c
	> Author: luoyi
	> Mail: 596890983@qq.com
	> Created Time: 2018年03月05日 星期一 21时37分36秒
 ************************************************************************/


#include<stdio.h>
#include "head.h"
#include "socket_create_connect.h"

int main() {
    int pid, i;
    char string[20] = "client_ip";
    for(i = 1; i <= 9; i++) {
        pid = fork();
        if(pid == 0) break;
    }
    if(i == 1) {
	        int sockfd, port;
	        char addr[20], port_str[20];
            string[strlen(string)] = i + '0';
	        get_conf_value("file", string, addr);
	        get_conf_value("file", "client_port1", port_str);
	        port = atoi(port_str);
            if(long_connect(port, addr, string, i) == -1) {
                perror("long_connect() error\n");
                return -1;
            }
    }/*else if(i == 2) {
        int sockfd, port;
	    char addr[20], port_str[20];
        string[strlen(string)] = i + '0';
	    get_conf_value("file", string, addr);
	    get_conf_value("file", "client_port1", port_str);
	    port = atoi(port_str);
        if(long_connect(port, addr, string, i) == -1) {
            perror("long_connect() error\n");
            return -1;
        }
    } else {
	    int sockfd, port;
	    char addr[20], port_str[20];
        string[strlen(string)] = i + '0';
	    get_conf_value("file", string, addr);
	    get_conf_value("file", "client_port1", port_str);
	    port = atoi(port_str);
        if(long_connect(port, addr, string, i) == -1) {
            perror("long_connect() error\n");
            return -1;
        }
    }*/
    exit(0);
    return 0;
}
