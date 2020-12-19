#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#ifndef S_BLKSIZE

#include <sys/param.h>
#define S_BLKSIZE DEV_BSIZE
#endif

enum errors 
{ 
    ARG_ERR = 1,
    STAT_ERR = 2    
};      

int main( int argc, char* argv[] )
{
    if ( argc != 2 )
    {
        printf("The program needs only one argument\nUsage: %s FILENAME\n", argv[0] );

        return ARG_ERR;
    }
    
    struct stat stat_buf;
    if ( lstat(argv[1], &stat_buf) == -1)
    {        
        perror("Failed to stat");

        return STAT_ERR;
    }

    printf("Path: \"%s\"\n", argv[1] );

    printf("Mode: ");

    switch (stat_buf.st_mode & S_IFMT) {
    case S_IFBLK:  printf("block device\n");            break;
    case S_IFCHR:  printf("character device\n");        break;
    case S_IFDIR:  printf("directory\n");               break;
    case S_IFIFO:  printf("FIFO/pipe\n");               break;
    case S_IFLNK:  printf("symlink\n");                 break;
    case S_IFREG:  printf("regular file\n");            break;
    case S_IFSOCK: printf("socket\n");                  break;
    default:       printf("unknown?\n");                break;
    }

    printf("Size: %li\n", stat_buf.st_size );
    printf("Used space: %li\n", stat_buf.st_blocks * S_BLKSIZE );

    printf("UID: %u\n", stat_buf.st_uid);
    printf("GID: %u\n", stat_buf.st_gid);

    printf("Last status change:       %s", ctime(&stat_buf.st_ctime));
    printf("Last file access:         %s", ctime(&stat_buf.st_atime));
    printf("Last file modification:   %s", ctime(&stat_buf.st_mtime));
  
    return 0;    
}