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

enum errors 
    { 
    ARG_ERR = 1,
    CRT_ERR = 2,
    SOURCE_ERR = 3,
    WRT_ERR = 4,
    DEST_ERR = 5,
    STAT_ERR = 6,
    MEM_ERR = 7,
    CHMOD_ERR = 8,
    UTIMENS_ERR = 9 
    }; 

int check( int code, const char* msg);

int main( int argc, char* argv[] )
{
    if ( argc != 2 )
    {
        printf("The program needs only one argument\nUsage: %s FILENAME\n", argv[0] );

        return ARG_ERR;
    }    
    struct statvfs stat_vbf;
    check( statvfs(argv[1], &stat_vbf), "failed to statvfs");

    size_t available = 0, occupied = 0, block = 0, all = 0;

    block = stat_vbf.f_frsize;
    all = stat_vbf.f_blocks*block;
    available = stat_vbf.f_bavail*block;
    occupied = (stat_vbf.f_blocks - stat_vbf.f_bfree)*block;

    printf("In byte: all - [%lu], available - [%lu], occupied - [%lu]\n", all, available, occupied ); 

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