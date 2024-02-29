#include <../include/allneed.h>
#include <../include/log.h>
int main(int argc, char *argv[])
{
    log_init();
    int listen_sock, client_sock;
    ssize_t readret;
    struct sockaddr_in addr, cli_addr;
    char buf[BUF_SIZE + 1000];
    memset(buf, 0, sizeof(buf));
    fprintf(stdout, "----- Echo Server -----\n");
    listen_sock = creat_socket();
    bind_socket(&addr, listen_sock);
    listen_socket(listen_sock, 32);
    int fds_arr[1024];
    int max_fd = -1;
    int coms_num;
    fd_set fds;
    for (int i = 0; i < 1024; i++)
        fds_arr[i] = -1;
    fds_arr[0] = listen_sock;
    while (1)
    {
        // printf("\nacceptiinnnnnnng\n");
        FD_ZERO(&fds);
        max_fd = 0;
        for (int i = 0; i < 1024; i++)
        {
            if (fds_arr[i] < 0)
                continue;
            FD_SET(fds_arr[i], &fds);
            max_fd = fds_arr[i] > max_fd ? fds_arr[i] : max_fd;
        }
        if ((coms_num = select(max_fd + 1, &fds, NULL, NULL, NULL)) < 0)
        {
            fprintf(stderr, "select() failure:%s\n", strerror(errno));
            close_socket(listen_sock, "door_sock");
            exit(EXIT_FAILURE);
        }
        if (FD_ISSET(fds_arr[0], &fds))
        {
            client_sock = accept_socket(listen_sock, &cli_addr);
            int i;
            for (i = 0; i < 1024; i++)
            {
                if (fds_arr[i] < 0)
                {
                    fds_arr[i] = client_sock;
                    break;
                }
            }
            if (i == 1024)
                close_socket(client_sock, "client_sock");
        }
        else
        {
            for (int k = 0; k < 1024; k++)
            {
                if (fds_arr[k] < 0 || !FD_ISSET(fds_arr[k], &fds))
                    continue;
                client_sock=fds_arr[k];
                if ((readret = recv(client_sock, buf, BUF_SIZE, 0)) <= 0)
                {
                    close_socket(fds_arr[k], "client_sock");
                    fds_arr[k] = -1;
                    continue;
                }
                int i = 0;
                while (1)
                {

                    int offset = 0;
                    char tmp[8192];
                    if (unpack(&i, &offset, tmp, buf, readret) != 4)
                    {
                        break;
                    }
                    // for(int i=0;i<tmpsize;i++)printf("%c",tmp[i]);
                    echocli(tmp, offset, listen_sock, client_sock, &cli_addr);
                    if (i == readret)
                        break;
                    // printf("haddlee end\n");
                }
                memset(buf, 0, BUF_SIZE);
                // printf("this recv end\n");

                // printf("recv=0,waitfor next accept\n");
                // test_recvfail(readret, client_sock, listen_sock);
                // close_socket(client_sock, "client_sock");
                fflush(pass);
                fflush(error);
            }
        }
    }

    close_socket(listen_sock, "door_sock");

    return EXIT_SUCCESS;
}
