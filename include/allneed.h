#pragma once
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include "parse.h"
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#define ECHO_PORT 9999
#define BUF_SIZE 8192
int file_istxt(char *filename);
int echo(int sock, int client_sock, char *buf, Request *request,struct sockaddr_in* cli_addr);
int echo_get(int sock, int client_sock, Request *request,struct sockaddr_in* cli_addr);
int echo_head(int sock, int client_sock, Request *request,struct sockaddr_in* cli_addr);
void not_imple(int sock,int client_sock, Request *request,struct sockaddr_in* cli_addr);
void bad_echo(int sock,int client_sock, Request *request,struct sockaddr_in* cli_addr);
void nosupport_echo(int sock,int client_sock, Request *request,struct sockaddr_in* cli_addr);
int close_socket(int sock,char* buf);
void bind_socket(struct sockaddr_in* addr,int sock);
int creat_socket();
void listen_socket(int sock,int listen_num);
int accept_socket(int sock, struct sockaddr_in *cli_addr);
void test_recvfail(int readret, int client_sock, int sock);
void echocli(char *tmp, int tmpsize, int sock,int client_sock,struct sockaddr_in* cli_addr);
int unpack(int* i,int* offset,char *tmp,char* buf,int readret);
