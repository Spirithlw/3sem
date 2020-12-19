#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

// USAGE: ./executable COPY_FROM COPY_TO

const int BUF_SIZE = 4096;

enum errors 
    { 
    ARG_ERR = 1,
    CRT_ERR = 2,
    SOURCE_ERR = 3,
    WRT_ERR = 4,
    DEST_ERR = 5,
    STAT_ERR = 6,
    MEM_ERR = 7 
    };

int main( int argc, char* argv[] )
{
    if ( argc != 3 )
        {
        printf("The program needs only two argument\nUSAGE: %s COPY_FROM COPY_TO\n", argv[0] );

        return ARG_ERR;
        }

    struct stat stat_buf;
    if ( lstat(argv[1], &stat_buf) == -1)
        {
        perror("Failed to stat\n");

        return STAT_ERR;
        }

    if( (stat_buf.st_mode & S_IFMT ) != S_IFREG  )
    {
        perror("Not a regular file\n");

        return ARG_ERR;
    }     

    int source_fd = open( argv[1], O_RDONLY);
        if (source_fd == -1 )
            {
            perror("Fail open source");
            close(source_fd);

            return SOURCE_ERR;
            }    

    int dest_fd = open( argv[2], O_WRONLY | O_TRUNC | O_CREAT , 0644);
        if ( dest_fd == -1 )
            {
            perror("Can't open destination file");
            close(source_fd);
            close(dest_fd);

            return DEST_ERR;
            }

        char buffer[BUF_SIZE] = {0};    
        size_t buf_read = -2;
        size_t written = 0;
        ssize_t res = 0;

        while( true )
        {
            
            buf_read = read( source_fd, buffer, BUF_SIZE );
            if ( buf_read == -1)
            {
                perror("failed to read a block");
            
                close(source_fd);
                close(dest_fd);
                return MEM_ERR;
            }

            if ( buf_read == 0 )
            {
                break;
            }
            
            while( written < buf_read)
            {

                res = write( dest_fd, (const void*) (buffer+written), buf_read - written );
                if (res == -1)
                {
                    perror("failed to write");
                    
                    close(source_fd);
                    close(dest_fd);                    

                    return WRT_ERR;
                }

                written += res; 

            }    

        written = 0;
        }  


    close(source_fd);
    close(dest_fd);

    return 0;   
}    