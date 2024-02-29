#include <../include/allneed.h>
#include <../include/log.h>

int echo(int sock, int client_sock, char *buf, Request *request, struct sockaddr_in *cli_addr)
{
    // char msg[BUF_SIZE];
    // memset(msg, 0, BUF_SIZE);
    // sprintf(msg,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n%s",strlen(buf),buf);
    if (send(client_sock, buf, strlen(buf), 0) != strlen(buf))
    {
        error_haddle(cli_addr, request);
        fprintf(error, "Error sending to client\n");
        // close_socket(client_sock, "client_sock");
        // close_socket(sock, "door_sock");
        // fclose(pass);
        // fclose(error);
        // exit(EXIT_FAILURE);
    }
    success_halddle(cli_addr, request, 200, strlen(buf));
    return 0;
}

int file_istxt(char *filename)
{
    int lf = strlen(filename);
    int i = 0;
    for (int i = 0; i < lf && filename[i] != '.'; i++)
        ;
    if (i != lf && strcmp(filename + i + 1, "txt") == 0)
        return 1;
    else
        return 0;
}

int echo_get(int sock, int client_sock, Request *request, struct sockaddr_in *cli_addr)
{
    char senbuf[4340];

    int fp;
    char srcfile[4096];
    if (strcmp(request->http_uri, "/") == 0)
    {
        strcpy(srcfile, "static_site/index.html");
    }
    else
    {
        strcpy(srcfile, "static_site/");
        strcat(srcfile, request->http_uri + 1);
    }

    fp = open(srcfile, O_RDONLY);
    if (fp == -1)
    {
        strcpy(senbuf, "HTTP/1.1 404 Not Found\r\n\r\n");
        error_haddle(cli_addr, request);
        fprintf(error, "404 File cannot open: %s\n", srcfile);
        if (send(client_sock, senbuf, strlen(senbuf), 0) != strlen(senbuf))
        {
            error_haddle(cli_addr, request);
            fprintf(error, "Error sending to client\n");
            // close_socket(client_sock, "client_sock");
            // close_socket(sock, "door_sock");
            // fclose(pass);
            // fclose(error);
            // close(fp);
            // exit(EXIT_FAILURE);
        }
    }
    else
    {
        struct stat s;
        stat(srcfile, &s);
        int senddsize = 0;
        sprintf(senbuf, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n", (int)s.st_size);
        senddsize = strlen(senbuf);
        // printf("\n\nsend=\n%s\n\n",senbuf);
        if (send(client_sock, senbuf, strlen(senbuf), 0) != strlen(senbuf))
        {
            error_haddle(cli_addr, request);
            fprintf(error, "Error sending to client\n");
            // close_socket(client_sock, "client_sock");
            // close_socket(sock, "door_sock");
            // fclose(pass);
            // fclose(error);
            // close(fp);
            // exit(EXIT_FAILURE);
        }
        int n = 0;
        while (1)
        {
            n = read(fp, senbuf, 1024);
            // printf("n=%d\n",n);
            // printf("n=%d\n",n);
            senddsize += n;
            if (n == 0)
                break;
            if (send(client_sock, senbuf, n, 0) != n)
            {
                error_haddle(cli_addr, request);
                fprintf(error, "Error sending to client\n");
                // close_socket(client_sock, "client_sock");
                // close_socket(sock, "door_sock");
                // close(fp);
                // fclose(pass);
                // fclose(error);
                // exit(EXIT_FAILURE);
            }
        }
        // printf("send success\n");

        success_halddle(cli_addr, request, 200, senddsize);
    }

    close(fp);
    return 0;
}

int echo_head(int sock, int client_sock, Request *request, struct sockaddr_in *cli_addr)
{
    char senbuf[1034];
    int fp;
    char srcfile[4096];
    if (strcmp(request->http_uri, "/") == 0)
    {
        strcpy(srcfile, "static_site/index.html");
    }
    else
    {
        strcpy(srcfile, "static_site/");
        strcat(srcfile, request->http_uri + 1);
    }
    fp = open(srcfile, O_RDONLY);
    if (fp == -1)
    {
        strcpy(senbuf, "HTTP/1.1 404 Not Found\r\n\r\n");
        error_haddle(cli_addr, request);
        fprintf(error, "404 File cannot open: %s\n", srcfile);
        if (send(client_sock, senbuf, strlen(senbuf), 0) != strlen(senbuf))
        {
            error_haddle(cli_addr, request);
            fprintf(error, "Error sending to client\n");
            // close_socket(client_sock, "client_sock");
            // close_socket(sock, "door_sock");
            // fclose(pass);
            // fclose(error);
            // close(fp);
            // exit(EXIT_FAILURE);
        }
    }
    else
    {
        struct stat s;
        stat(srcfile, &s);
        int senddsize = 0;
        sprintf(senbuf, "HTTP/1.1 200 OK\r\n\r\n");
        senddsize = strlen(senbuf);
        if (send(client_sock, senbuf, strlen(senbuf), 0) != strlen(senbuf))
        {
            error_haddle(cli_addr, request);
            fprintf(error, "Error sending to client\n");
            // close_socket(client_sock, "client_sock");
            // close_socket(sock, "door_sock");
            // close(fp);
            // fclose(pass);
            // fclose(error);
            // exit(EXIT_FAILURE);
        }
        success_halddle(cli_addr, request, 200, senddsize);
    }
    close(fp);
    return 0;
}

void not_imple(int sock, int client_sock, Request *request, struct sockaddr_in *cli_addr)
{
    char buffer[4096];
    strcpy(buffer, "HTTP/1.1 501 Not Implemented\r\n\r\n");
    error_haddle(cli_addr, request);
    fprintf(error, "Not Implemented\n");
    if (send(client_sock, buffer, strlen(buffer), 0) != strlen(buffer))
    {
        error_haddle(cli_addr, request);
        fprintf(error, "Error sending to client\n");
        // close_socket(client_sock, "client_sock");
        // close_socket(sock, "door_sock");
        // fclose(pass);
        // fclose(error);
        // exit(EXIT_FAILURE);
    }
}

void bad_echo(int sock, int client_sock, Request *request, struct sockaddr_in *cli_addr)
{
    char buffer[4096];
    time_t cur_time;
    time(&cur_time);
    struct tm *curtm = localtime(&cur_time);
    strcpy(buffer, "HTTP/1.1 400 Bad request\r\n\r\n");
    fprintf(error, "[%.2d/%s/%.4d:%.2d:%.2d:%.2d +0800] [ERROR] [client %s]  ",
            curtm->tm_mday, zhmon(curtm->tm_mon), curtm->tm_year + 1900, curtm->tm_hour,
            curtm->tm_min, curtm->tm_sec, inet_ntoa(cli_addr->sin_addr));
    fprintf(error, "Cannot haddle Bad request\n");
    if (send(client_sock, buffer, strlen(buffer), 0) != strlen(buffer))
    {
        fprintf(error, "[%.2d/%s/%.4d:%.2d:%.2d:%.2d +0800] [ERROR] [client %s]  ",
                curtm->tm_mday, zhmon(curtm->tm_mon), curtm->tm_year + 1900, curtm->tm_hour,
                curtm->tm_min, curtm->tm_sec, inet_ntoa(cli_addr->sin_addr));
        fprintf(error, "Error sending to client\n");
        // close_socket(client_sock, "client_sock");
        // close_socket(sock, "door_sock");
        // fclose(pass);
        // fclose(error);
        // exit(EXIT_FAILURE);
    }
}

void nosupport_echo(int sock, int client_sock, Request *request, struct sockaddr_in *cli_addr)
{
    char buffer[4096];
    strcpy(buffer, "HTTP/1.1 505 HTTP Version not supported\r\n\r\n");
    error_haddle(cli_addr, request);
    fprintf(error, "HTTP Version not supported\n");
    if (send(client_sock, buffer, strlen(buffer), 0) != strlen(buffer))
    {
        error_haddle(cli_addr, request);
        fprintf(error, "Error sending to client\n");
        // fclose(pass);
        // fclose(error);
        // close_socket(client_sock, "client_sock");
        // close_socket(sock, "door_sock");
        // exit(EXIT_FAILURE);
    }
}

int unpack(int* i,int* offset,char *tmp,char* buf,int readret)
{
    enum
    {
        STATE_START = 0,
        STATE_CR,
        STATE_CRLF,
        STATE_CRLFCR,
        STATE_CRLFCRLF
    };
    int status = STATE_START;
    char ch;
    while (status != STATE_CRLFCRLF)
    {
        char expected = 0;
        if (*i == readret)
            break;
        ch = buf[(*i)++];
        tmp[(*offset)++] = ch;
        switch (status)
        {
        case STATE_START:
        case STATE_CRLF:
            expected = '\r';
            break;
        case STATE_CR:
        case STATE_CRLFCR:
            expected = '\n';
            break;
        default:
            status = STATE_START;
            continue;
        }
        if (ch == expected)
            status++;
        else
            status = STATE_START;
    }
    return status;
}

void echocli(char *tmp, int tmpsize, int sock,int client_sock,struct sockaddr_in* cli_addr)
{
    Request *request = parse(tmp, tmpsize, client_sock);

    if (request == NULL)
        bad_echo(sock, client_sock, NULL, cli_addr);
    else
    {
        if (strcmp(request->http_method, "GET") == 0)
        {
            // if (echo(sock, client_sock, buf, readret) == EXIT_FAILURE)
            //     return EXIT_FAILURE;
            if (strcmp(request->http_version, "HTTP/1.1") != 0)
                nosupport_echo(sock, client_sock, request, cli_addr);
            else
            {
                echo_get(sock, client_sock, request, cli_addr);
            }
        }
        else if (strcmp(request->http_method, "HEAD") == 0)
        {
            if (strcmp(request->http_version, "HTTP/1.1") != 0)
                nosupport_echo(sock, client_sock, request, cli_addr);
            else
            {
                echo_head(sock, client_sock, request, cli_addr);
            }
        }
        else if (strcmp(request->http_method, "POST") == 0)
        {
            if (strcmp(request->http_version, "HTTP/1.1") != 0)
                nosupport_echo(sock, client_sock, request, cli_addr);
            else
            {
                echo(sock, client_sock, tmp, request, cli_addr);
            }
        }

        else
        {
            not_imple(sock, client_sock, request, cli_addr);
        }
    }
}
