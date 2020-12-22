#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <wait.h>
#include <mqueue.h>
#include <errno.h>

enum errors 
    { 
    ARG_ERR     = 1,
    MQ_ERR    = 2,
    DIR_ERR     = 3,
    DRFD_ERR    = 4 
    };

int main (int argc, char *argv[])
{
    if (argc != 2) 
    {
        printf("Usage: %s mq_name\n", argv[0]);

        return ARG_ERR;
    }
    bool is_mq_open = 0; 
    char* mq_name = argv[1];
    printf(" queue_name: '%s'\n", mq_name);
    
    mqd_t mq = mq_open(mq_name, O_RDONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, NULL);
    if ( (mq == -1) && (errno == EEXIST) ) 
    {
        printf("mq %s already exists\n", mq_name);
        mq = mq_open(mq_name, O_RDONLY);
        is_mq_open = 1;
    }

    if (mq == -1)
    {
        perror("failed to open or create mq");

        return MQ_ERR;
    }
    
    struct mq_attr mq_attributes = {0};
    mq_getattr(mq, &mq_attributes);

    if (mq_attributes.mq_flags)
    {
        printf ("mq flag is O_NONBLOCK\n");
    }    
    else 
    {
        printf ("mq flag is 0\n");
    }

    printf ("Maximum # of messages on queue: %ld\n", mq_attributes.mq_maxmsg);
    printf ("Maximum message size: %ld\n", mq_attributes.mq_msgsize);
    printf ("# of messages now on queue: %ld\n", mq_attributes.mq_curmsgs);

    mq_close (mq);
    if (!is_mq_open) 
    {
        mq_unlink (mq_name);
    }
    return 0;
}