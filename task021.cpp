#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

// USAGE: ./executable FILENAME WHAT_TO_WRITE

enum errors 
    { 
    ARG_ERR = 1,
    CRT_ERR = 2,
    SOURCE_ERR = 3,
    WRT_ERR = 4,
    CLOSE_ERR = 5 
    };  


int main( int argc, char* argv[] )
    {
    if ( argc != 3 )
        {
        printf("The program needs only two argument\nUsage: %s FILENAME WHAT_TO_WRITE\n", argv[0] );

        return ARG_ERR;
        }
    
    int create_fd = open( argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0600 ); // create();
    if ( create_fd < 0 )
        {
        perror( "Can't create file" );
        close( create_fd );

        return CRT_ERR;
        }  

    size_t count = strlen( argv[2] );
    size_t written = 0;
    ssize_t res = 0;

    while ( written < count )
        {
        res = write( create_fd, (const void*) (argv[2]+written), count - written );
        if ( res == -1 )
            {
            perror("write error");

            close( create_fd );
            return WRT_ERR;
            }

        written += res;
        }
        
    if( close( create_fd ) == -1)
        {
        perror("close error");

        return CLOSE_ERR;    
        }                

    return 0;    
    }