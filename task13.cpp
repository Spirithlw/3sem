#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <wait.h>

int check( int code);

int main( int argc, char** argv )
{
    int curr_pid = check( fork() );

    if ( curr_pid == 0 )
    {
        //child
        printf("Command: %s - child\nPID: %u\nPPID: %u\n\n", argv[0], getpid(), getppid() );

        return 0;
    }
    else 
    {
        //parent
        int status = 0;
        int ret_code = wait(&status);
        if ( ret_code == -1 )
        {
            perror("wait");
            exit(EXIT_FAILURE);
        }
        printf("Command: %s - parent\nPID: %u\nPPID: %u\n", argv[0], getpid(), getppid() );
        printf("Child terminated with status: %d\n", status );

        return 0;
    }

    printf("\n");
    return 0;
}

int check( int code)
{
    if ( code == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    return code;
}