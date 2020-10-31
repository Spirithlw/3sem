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
    WRT_ERR = 4 
    };  


int main( int argc, char* argv[] )
    {
    if ( argc != 3 )
        {
        printf("The program needs only two argument\n");

        return ARG_ERR;
        }
    
    int create_fd = open( argv[1], O_CREAT | O_RDWR | O_TRUNC, 0600 ); // create();
    if ( create_fd < 0 )
        {
        perror( "Can't create file" );
        close( create_fd );

        return CRT_ERR;
        }  

    long count = strlen( argv[2] );
    long res = write( create_fd, (const void*) argv[2], count );

    if ( res != count )
        {
            perror( "message wrote incorrectly" );
            close ( create_fd );

            return WRT_ERR;
        }
        
    close( create_fd );                

    return 0;    
    }