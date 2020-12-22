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
    GRP_ERR    = 12 
    };


int check( int code, const char* msg);

int main( int argc, char** argv )
{
    printf("Command: %s\nPID: %u\nPPID: %u\nPGID: %u\nSID: %u\n", argv[0], getpid(), getppid(), 
    getpgid(getpid()), getsid(getpid()) );

    struct passwd* pwd = getpwuid( getuid() ); 
    if (pwd == nullptr )
    {
        perror( "Error");

        return MEM_ERR;
    }
    printf("UID: %u\nUNAME: %u\nGID: %u\n", pwd->pw_uid, pwd->pw_name, pwd->pw_gid );

    mode_t mask = umask(0);
    printf("mask: %d\n", (int) mask );
    umask(mask);


    struct group* grp = getgrgid( pwd->pw_gid ); 
    if (pwd == nullptr )
    {
        perror( "Error");

        return MEM_ERR;
    }
    char* buf = get_current_dir_name();
    if ( buf == nullptr )
    {
        perror("Error");

        return MEM_ERR;
    }

    printf("CWD: %s\nusername: %s\nhome dir: %s\n", buf, pwd->pw_dir);

    printf("Group name: %s\nGroup ID: %u\n", grp->gr_name, grp->gr_gid);
    gid_t* curr_group = nullptr;
    size_t group_count = 0;
    size_t ngroups_max = 0;

    ngroups_max = sysconf(_SC_NGROUPS_MAX) + 1;
    curr_group = (gid_t *) calloc(ngroups_max , sizeof(gid_t) );

    group_count = getgroups(ngroups_max, curr_group);
    printf("groups:\n");
    struct group *grps;
    for (int i = 0; i < group_count; i++)
    {
        printf("%u", curr_group[i]);
        if (!( grps = getgrgid(curr_group[i]) ) )
        {
            printf("error %s;", grps->gr_name);

            return GRP_ERR;
        }
        printf(" %s\n", grps->gr_name);

    }    
    printf("\n");

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

