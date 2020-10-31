#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

enum errors 
    { 
    ARG_ERR = 1,
    STAT_ERR = 2 
    };  

int main( int argc, char* argv[] )
    {
    if ( argc != 2 )
        {
        printf("The program needs only one argument\n");

        return ARG_ERR;
        }
    
    struct stat stat_buf;
    if ( lstat(argv[1], &stat_buf) == -1)
        {
        perror("Failed to stat");

        return STAT_ERR;
        }

    printf( "Mode: %o\n", stat_buf.st_mode );
    printf("Size: %li\n", stat_buf.st_size );
    printf("Used space: %li\n", stat_buf.st_size * 512 );  


    return 0;    
    }