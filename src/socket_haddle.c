#include <../include/allneed.h>


int close_socket(int sock,char* buf)
{
    if (close(sock))
    {
        fprintf(stderr, "Failed closing %s.\n",buf);
        exit(EXIT_FAILURE);
    }
    return 0;
}


int creat_socket()
{
    int sock;
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "Failed creating socket.\n");
        exit(EXIT_FAILURE);
    }
    int bReuseaddr = 1;
    setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, &bReuseaddr, sizeof( bReuseaddr ) );

    return sock;
}

void bind_socket(struct sockaddr_in *addr, int sock)
{
    addr->sin_family = AF_INET;
    addr->sin_port = htons(ECHO_PORT);
    addr->sin_addr.s_addr = INADDR_ANY;
    if (bind(sock, (struct sockaddr *)addr, sizeof(struct sockaddr_in)))
    {
        close_socket(sock,"door_sock");
        fprintf(stderr, "Failed binding socket.\n");
        exit(EXIT_FAILURE);
    }
}

void listen_socket(int sock, int listen_num)
{
    if (listen(sock, listen_num))
    {
        close_socket(sock,"door_sock");
        fprintf(stderr, "Error listening on socket.\n");
        exit(EXIT_FAILURE);
    }
}

int accept_socket(int sock, struct sockaddr_in *cli_addr)
{
    socklen_t cli_size = sizeof(struct sockaddr);
    int client_sock;
    if ((client_sock = accept(sock, (struct sockaddr *)cli_addr, &cli_size)) == -1)
    {
        close(sock);
        fprintf(stderr, "Error accepting connection.\n");
        exit(EXIT_FAILURE);
    }
    return client_sock;
}

void test_recvfail(int readret, int client_sock, int sock)
{
    if (readret == -1)
    {
        close_socket(client_sock,"client_sock");
        close_socket(sock,"door_sock");
        fprintf(stderr, "Error reading from client socket.\n");
        exit(EXIT_FAILURE);
    }
}


