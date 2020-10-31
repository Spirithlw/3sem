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

int check( int code, const char* msg);

int main( int argc, char** argv )
{
    struct statfs stat_bf;
    struct statvfs stat_vbf;

    check( statfs(argv[1], &stat_bf), "statfs");
    check( statvfs(argv[1], &stat_vbf), "statvfs");

    unsigned long free = 0, available = 0, occupied = 0, block = 0;
    block = stat_vbf.f_bsize;
    free = stat_vbf.f_bfree*block;
    available = stat_vbf.f_bavail*block;
    occupied = (stat_bf.f_blocks - stat_vbf.f_bfree)*block;

    printf("In byte: free - [%lu], available - [%lu], occupied - [%lu]\n", free, available, occupied ); 

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