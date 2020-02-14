#ifndef SHARING_H
#define SHARING_H

#include <iostream> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

namespace extend
{

void shareFunc(bool write, const char * message)
{
    key_t key;
    int shmid;
    int mode;
    int SHM_SIZE = 1024;

    /* make the key: */
    if ((key = ftok("hello.txt", 'R')) == -1) /*Here the file must exist */ 
    {
        perror("ftok");
        exit(1);
    }

    /*  create the segment: */
    if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }

    /* attach to the segment to get a pointer to it: */
    void * data = shmat(shmid, NULL, 0);

    if (data == (char *)(-1)) {
        perror("shmat");
        exit(1);
    }

    if (write) {
        printf("writing to segment: \"%s\"\n", message);
        strncpy((char *)data, message, SHM_SIZE);
    } else
        printf("segment contains: \"%s\"\n", data);

    /* detach from the segment: */
    if (shmdt(data) == -1) {
        perror("shmdt");
        exit(1);
    }

    //delete shared mem
   // shmctl(shmid, IPC_RMID, NULL);

}

}
#endif