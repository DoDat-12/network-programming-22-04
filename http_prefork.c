#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>

int main()
{
    // create socket
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
    {
        perror("socket() failed: ");
        exit(1);
    }
    printf("create socket successfully\n");

    // address struct
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(7000);

    // binding
    if (bind(listener, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
    {
        perror("bind() failed: ");
        exit(1);
    }
    printf("binding socket successfully\n");

    // ready
    if (listen(listener, 5) == -1)
    {
        perror("listen() failed: ");
        exit(1);
    }
    printf("waiting for connection...\n");

    char buf[1024];
    int num_processes = 8;

    for (int i = 0; i < num_processes; i++)
    {
        if (fork() == 0)
        {
            char buf[256];
            while (1)
            {
                int client = accept(listener, NULL, NULL);
                int ret = recv(client, buf, sizeof(buf), 0);
                if (ret <= 0)
                {
                    close(client);
                    continue;
                }

                if (ret < sizeof(buf))
                    buf[ret] = 0;

                printf("%s\n", buf);

                char msg[] = "HTTP/1.1 200OK\r\nContent-Type : text/html\r\n\r\n<html><head><style>body {background-color: powderblue;}\nh1 {color: blue;}\np {color: red;}</style></head><body>\n<h1>DODAT</h1>\n<p>Homework 22-04</p>\n</body></html>";
                send(client, msg, strlen(msg), 0);

                close(client);
            }
            exit(0);
        }
    }

    getchar();
    close(listener);
    killpg(0, SIGKILL);
    return 0;
}