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
#include <dirent.h>

enum errors 
    { 
    ARG_ERR     = 1,
    CRT_ERR     = 2,
    SOURCE_ERR  = 3,
    WRT_ERR     = 4,
    DEST_ERR    = 5,
    STAT_ERR    = 6,
    MEM_ERR     = 7,
    CHMOD_ERR   = 8,
    UTIMENS_ERR = 9,
    OPEN_ERR    = 10,
    CLS_ERR     = 11,
    DRFD_ERR    = 12 
    };

const char* file_type(mode_t file_mode) 
{
    	    switch (file_mode & S_IFMT) 
        {
		    case S_IFBLK:   return "block device";      break;
		    case S_IFCHR:   return "character device";  break;
		    case S_IFDIR:   return "directory";         break;
		    case S_IFIFO:   return "FIFO/pype";         break;
		    case S_IFLNK:   return "symlink";           break;
		    case S_IFREG:   return "regular file";      break;
		    case S_IFSOCK:  return "socket";            break;
		    default:        return "unknown";           break;
	    }
    }  

const char* dir_type(unsigned char type)
{
    switch (type)
    {
        case DT_FIFO:   return "FIFO";                  break;
        case DT_CHR:    return "CHARACTER SPECIAL";     break;
        case DT_DIR:    return "DIRECTORY";             break;
        case DT_BLK:    return "BLOCK SPECIAL";         break;
        case DT_REG:    return "REGULAR";               break;
        case DT_LNK:    return "SYMBOLIC LINK";         break;
        case DT_SOCK:   return "SOCKET";                break;
        case DT_WHT:    return "WHITEOUT";              break;
        default:        return "unknown?";              break;
    }
}      

int main( int argc, char* argv[] )
{

    const char* path = nullptr;

    if ( argc > 2 )
    {
        printf("The program requires not more than one argument\nUsage: %s"
               " DIRNAME(if no, using current dir)\n", argv[0] );

        return ARG_ERR;       
    }

    if ( argc == 1 )
    {
        path = ".";
    }
    
    if( argc == 2 )
    {
        path = argv[1];
    }

    DIR* dir_ptr = opendir(path);
    if ( dir_ptr == nullptr)
    {
        perror("failed to open directory");

        return OPEN_ERR;
    } 

    int dir_fd = dirfd(dir_ptr);
	if ( dir_fd == -1)
	{
		perror("failed to dirfd ");

		return DRFD_ERR;
	}

    struct dirent* dir_str = nullptr;
    struct stat stat_buf;
    printf("directory: %s\n", path);
    printf("%-25s%s\n", "Type", "Name");

    ssize_t length = strlen(path) + 256;
    char* path_buf = (char*) calloc(length, sizeof(char) );

    while( (dir_str = readdir(dir_ptr)) != nullptr )
    {
        sprintf( path_buf, "%s/%s", path, dir_str->d_name);
        if (dir_str->d_type == DT_UNKNOWN )
        {
            if ( fstatat(dir_fd, dir_str->d_name, &stat_buf, AT_SYMLINK_NOFOLLOW) == -1)
            {        
                perror("failed to stat");

                return STAT_ERR;
            }
            printf("%-25s", file_type(stat_buf.st_mode) );
        }
        else 
        {
            printf("%-25s", dir_type(dir_str->d_type) );    
        }
        printf("%s\n", dir_str->d_name );

    }

    free(path_buf);
    if ( closedir(dir_ptr) == -1) 
    {
        perror("failed to close");

        return  CLS_ERR;
    }
    
    return 0;
}