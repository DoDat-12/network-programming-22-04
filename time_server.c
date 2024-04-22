#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <time.h>

int main()
{
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
    {
        perror("socket() failed\n");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)))
    {
        perror("bind() failed\n");
        return 1;
    }

    if (listen(listener, 5))
    {
        perror("listen() failed\n");
        return 1;
    }

    int num_processes = 8;
    for (int i = 0; i < num_processes; i++)
    {
        if (fork() == 0)
        {
            char buf[256];
            char rep[256];

            // clock
            time_t rawtime;
            struct tm *timeinfo;
            while (1)
            {
                int client = accept(listener, NULL, NULL);
                printf("New client connected: %d\n", client);
                int ret = recv(client, buf, sizeof(buf), 0);
                if (ret <= 0)
                    continue;
                buf[ret] = 0;
                printf("Received from %d: %s", client, buf);

                time(&rawtime);
                timeinfo = localtime(&rawtime);
                // Set to Vietnam (UTC + 7)
                timeinfo->tm_hour += 7;
                if (timeinfo->tm_hour >= 24)
                {
                    timeinfo->tm_hour -= 24;
                    timeinfo->tm_mday += 1;
                }

                if (strncmp(buf, "GET_TIME dd/mm/yyyy", 19) == 0)
                    sprintf(rep, "Date: %02d/%02d/%04d", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);
                else if (strncmp(buf, "GET_TIME dd/mm/yy", 17) == 0)
                    sprintf(rep, "Date: %02d/%02d/%02d", timeinfo->tm_mday, timeinfo->tm_mon + 1, (timeinfo->tm_year + 1900) % 100);
                else if (strncmp(buf, "GET_TIME mm/dd/yyyy", 19) == 0)
                    sprintf(rep, "Date: %02d/%02d/%04d", timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_year + 1900);
                else if (strncmp(buf, "GET_TIME mm/dd/yy", 17) == 0)
                    sprintf(rep, "Date: %02d/%02d/%02d", timeinfo->tm_mon + 1, timeinfo->tm_mday, (timeinfo->tm_year + 1900) % 100);
                else
                    sprintf(rep, "Wrong command");

                send(client, rep, strlen(rep), 0);
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

// nc -v localhost 9000