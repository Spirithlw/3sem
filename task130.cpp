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
    pid_t curr_pid = check( fork() );

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
        int ret_code = waitpid( curr_pid, &status, 0 );
        if ( ret_code == -1 )
        {
            perror("wait");
            exit(EXIT_FAILURE);
        }
        if (WIFEXITED(status)) 
        {
        printf("exited, status=%d\n", WEXITSTATUS(status));
        } else 
            if (WIFSIGNALED(status)) 
            {
               printf("killed by signal %d\n", WTERMSIG(status));         
            } else 
            if (WIFSTOPPED(status))           
            {
                printf("stopped by signal %d\n", WSTOPSIG(status));
            } else 
                if (WIFCONTINUED(status)) 
                {
                    printf("continued\n");
                }


        printf("Command: %s - parent\nPID: %u\nPPID: %u\n", argv[0], getpid(), getppid() );
        printf("Child terminated with status: %d\n", status );

        return 0;
    }

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