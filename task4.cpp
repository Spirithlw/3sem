#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <wait.h>
#include <sys/statvfs.h>
#include <sys/statfs.h>

//USAGE: ./exec     COPY_FROM   COPY_TO

int check( int code, const char* msg);

int main( int argc, char** argv )
{
    struct stat stat_buf;
    check( lstat(argv[1], &stat_buf), "lstat");

    int create_fd = open( argv[2], O_RDONLY, 0444);

    check( fchmod(create_fd, stat_buf.st_mode), "fchmod");

    struct timespec time_array[2] = {stat_buf.st_atim, stat_buf.st_mtim};
    check( futimens(create_fd, time_array) , "futimens");

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