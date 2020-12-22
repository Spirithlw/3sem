#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <wait.h>

int check( int code, const char* msg);

int main( int argc, char** argv )
{
    int pipefd[2] = {0};
    pipe(pipefd);
    pid_t curr_pid = fork();
    check( curr_pid, "fork" );

    if ( curr_pid == 0 )
    {
        //child
        check( close(pipefd[1]), "close" );
        printf("Command: %s - child\nPID: %u\nPPID: %u\n\n", argv[0], getpid(), getppid() );
        int ping = 0;
        check( read( pipefd[0], &ping, sizeof(ping) ), "read" );
        check( close(pipefd[0]), "close");
        printf("Command: %s - child\nPID: %u\nPPID: %u\n\n", argv[0], getpid(), getppid() );
    }
     else 
    {
        //parent
        check( close(pipefd[0]), "close");
        sleep(1);
        int ping = 0xDEADBEEF;
        check( write( pipefd[1], &ping, sizeof(ping) ), "write" );
        check( close(pipefd[1]), "close");

    }

    return 0;
}

int check( int code, const char* msg)
{
    if ( code == -1)
    {
        perror(msg);
        exit(EXIT_FAILURE);
    }

    return code;
}