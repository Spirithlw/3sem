#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//3.1копирование содержимого обычных(regular) файлов - task
//lstat+open+read+write+close
//3.2реализовать эту же таску через pread/pwrite, разобраться в отличиях
//3.3sendfile
//splice
//copy_file_range
//3* добавить обработку FIFO(mkfifo) символических ссылок symlink файлов устройств mknod
//4копирование базовых метаданных
//права (fchmod)
//atime mtime  futimens 
//знать про недостатки utime/utimes
//5* копирование UID,GID,ACL,attr, xattr(user_xattr), SElinux security labels 

int main()
    {

    while ( true )
        {

        bytes_read = read(  );

        bytes_written = 0;
        while ()
            {
            res = write(...);

            bytes_written += res;
            write( dst_fd, &buffer[bytes_written], bytes_read - bytes_written);


            }
        
        }

    return 0;
    }