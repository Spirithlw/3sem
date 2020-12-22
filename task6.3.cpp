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
    STAT_ERR    = 2,
    DIR_ERR     = 3,
    DRFD_ERR    = 4 
    };

const char* file_type(mode_t file_mode) 
{
    	    switch (file_mode & S_IFMT) 
        {
		    case S_IFBLK:   return "block device";      
		    case S_IFCHR:   return "character device";  
		    case S_IFDIR:   return "directory";         
		    case S_IFIFO:   return "FIFO/pype";         
		    case S_IFLNK:   return "symlink";           
		    case S_IFREG:   return "regular file";
            case S_IFSOCK:  return "socket";        
		    default:        return "unknown";           
	    }
    }  

const char* dir_type(unsigned char type)
{
    switch (type)
    {
        case DT_FIFO:   return "FIFO";                  
        case DT_CHR:    return "CHARACTER SPECIAL";     
        case DT_DIR:    return "DIRECTORY";             
        case DT_BLK:    return "BLOCK SPECIAL";         
        case DT_REG:    return "REGULAR";               
        case DT_LNK:    return "SYMBOLIC LINK";         
        case DT_SOCK:   return "SOCKET";                
        case DT_WHT:    return "WHITEOUT";              
        default:        return "unknown?";              
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

    struct dirent* dir_str = nullptr;
    struct stat stat_buf;
    int shift = 0;
    loff_t base = 0;
    int dir_fd = open( path, O_RDONLY );
    ssize_t count = 0;
    char* buffer = (char*) calloc( 16, sizeof(struct dirent) );
    if ( dir_fd == -1)
	{
		perror("failed to open ");

		return DRFD_ERR;
	}

    printf("directory: %s\n", path);
    printf("%-25s%s\n", "Type", "Name");

    while( true )
    {
        base = 0;
        count = getdirentries(dir_fd, buffer, 16*sizeof(struct dirent), &base);
        if ( count == -1 )
        {
            perror("failed to getdirentries");

            return DIR_ERR;
        }

        if ( count == 0 )
        {
            break;
        }

        while( shift < count )
        {   
            dir_str = (struct dirent* )(buffer + shift);
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
            shift+= dir_str->d_reclen;
        }

        shift = 0;
    }

    close(dir_fd);
    free(buffer);
    return 0;
}
