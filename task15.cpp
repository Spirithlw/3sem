#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <wait.h>
#include <signal.h>

int RECEIVED_SIGNAL;

enum errors 
    { 
    ARG_ERR     = 1,
    CRT_ERR     = 2,
    SOURCE_ERR  = 3,
    WRT_ERR     = 4,
    DEST_ERR    = 5,
    STAT_ERR    = 6,
    MEM_ERR     = 7,
    SIGN_ERR   = 8,
    UTIMENS_ERR = 9,
    OPEN_ERR    = 10,
    CLS_ERR     = 11,
    DRFD_ERR    = 12 
    };

void signal_handler(int code)
{
    RECEIVED_SIGNAL = code;
}

int main()
{
    struct sigaction sigact;
    sigact.sa_handler = signal_handler;

    if (sigaction(SIGINT, &sigact, NULL ) == -1)
    {
        perror("failed to SIGINT");
        return SIGN_ERR;
    }
    if (sigaction(SIGQUIT, &sigact, NULL ) == -1)
    {
        perror("failed to SIGQUIT");
        return SIGN_ERR;
    }
    if (sigaction(SIGTERM, &sigact, NULL ) == -1)
    {
        perror("failed to SIGTERM");
        return SIGN_ERR;
    }
    while( true )
    {
        printf("im alive\n");
        pause();
        switch (RECEIVED_SIGNAL)
        {
            case SIGINT:
                printf("SIGINT received\n");
                break;
            case SIGQUIT:
                printf("SIGQUIT received\n");
                break;
            case SIGTERM:
                printf("SIGTERM received\n");
                break;
            default:
                printf("unhandled SIGNAL");
                break;
        }
    }

    return 0;
}