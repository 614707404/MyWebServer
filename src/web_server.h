//
// Created by 戴威涛 on 2022/3/4.
//

#ifndef MYWEBSERVER_WEB_SERVER_H
#define MYWEBSERVER_WEB_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

void server_pro(int argc, char *argv[]);
void* request_handler(void* arg);
//void send_data(FILE* fp, char* ct, char* file_name);
void send_data(FILE* fp);
char* content_type(char* file);
void send_error(FILE* fp);
void error_handling(char* message);

#endif //MYWEBSERVER_WEB_SERVER_H
