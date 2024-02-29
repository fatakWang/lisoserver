#pragma once
FILE *error;
FILE *pass;
int log_init();
char *zhmon(int mon);
void success_halddle(struct sockaddr_in *cli_addr,Request *request,int status,int sendsize);
void error_haddle(struct sockaddr_in *cli_addr,Request *request);