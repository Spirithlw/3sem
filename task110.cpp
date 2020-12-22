#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/file.h>
#include <fcntl.h>

#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)
#define STRLINE STRINGIFY(__LINE__)
#define TRACE "Line: " STRLINE " "

const int BUFF_SZ = 20;
const char filename[] = "counter.txt";

int check(int code, const char* msg);

long long str_to_ll(char* str);

int main()
{
    int fd = check(open(filename, O_RDWR | O_CREAT, 0666), TRACE "open");
    check(flock(fd, LOCK_EX), TRACE "flock");

    char buff[BUFF_SZ] = "";
    check(read(fd, buff, sizeof(BUFF_SZ)), TRACE "read");

    check(lseek(fd, 0, SEEK_SET), TRACE "lseek");
    sprintf(buff, "%lld", str_to_ll(buff) + 1);
    check(write(fd, buff, BUFF_SZ), TRACE "write");
    
    check(flock(fd, LOCK_UN), TRACE "flock");
    check(close(fd), TRACE "close");

    return 0;
}

int check(int code, const char* msg)
{
    if (code == -1)
    {
        perror(msg);
        exit(EXIT_FAILURE);
    }
    return code;
}

long long str_to_ll(char* str)
{
    assert(str);

    char* endptr = NULL;
    long long ret_val = strtoll(str, &endptr, 10);

    if (*endptr != '\0')
    {
        fprintf(stderr, "Conversion error %s. Invalid symbol: %c\n", str, *endptr);
        exit(1);
    }
    if (ret_val == LLONG_MAX && errno == ERANGE)
    {
        fprintf(stderr, "Overflow occured\n");
        exit(1);
    }
    if (ret_val == LLONG_MIN && errno == ERANGE)
    {
        fprintf(stderr, "Underflow occured\n");
        exit(1);
    }
    return ret_val;
}