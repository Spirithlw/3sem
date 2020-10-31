#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <wait.h>
#include <pwd.h>
#include <grp.h>
#include <sys/time.h>
#include <sys/resource.h>


int check( int code, const char* msg);

int main( int argc, char** argv )
{
    printf("Command: %s\nPID: %u\nPPID: %u\n", argv[0], getpid(), getppid() );

    struct passwd* pwd = getpwuid( getuid() ); 
    gid_t supp_id = getgid();
    getgroups(1, &supp_id);

    printf("CWD: %s\nusername: %s\nhome dir: %s\n"
    "UID: %u\nGID: %u\nFirst supplementary gid: %u\n", get_current_dir_name(), pwd->pw_name, pwd->pw_dir, 
    pwd->pw_uid, pwd->pw_gid, supp_id );

    struct group* grp = getgrgid( getgid() );
    printf("Group name: %s\nGroup ID: %u\n", grp->gr_name, grp->gr_gid);

    printf("SCHED\nGetpriority: %d\n", getpriority(PRIO_PROCESS, 0)  );

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