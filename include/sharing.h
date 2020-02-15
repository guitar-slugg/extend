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

int createSharedMemory(const char *fileKey, int shm_size)
{
    key_t key;
    int shmid;
    /* make the key: */
    if ((key = ftok(fileKey, 'R')) == -1) /*Here the file must exist */
    {
        perror("ftok");
        exit(1);
    }

    /*  create the segment: */
    if ((shmid = shmget(key, shm_size, 0644 | IPC_CREAT)) == -1)
    {
        perror("shmget");
        exit(1);
    }

    return shmid;
};


void writeSharedMem(int shmid, const char * message, int shm_size)
{
    /* attach to the segment to get a pointer to it: */
    void *data = shmat(shmid, NULL, 0);
    if (data == (char *)(-1))
    {
        perror("shmat");
        exit(1);
    }

    //copy in data 
    strncpy((char *)data, message, shm_size);

    /* detach from the segment: */
    if (shmdt(data) == -1)
    {
        perror("shmdt");
        exit(1);
    }
}

void readSharedMem(int shmid, char * buffer, int shm_size)
{
    /* attach to the segment to get a pointer to it: */
    void *data = shmat(shmid, NULL, 0);
    if (data == (char *)(-1))
    {
        perror("shmat");
        exit(1);
    }

    //copy data 
    strncpy(buffer, (char *)data, shm_size);

    /* detach from the segment: */
    if (shmdt(data) == -1)
    {
        perror("shmdt");
        exit(1);
    }
}

void clearSharedMem(int shmid)
{
    shmctl(shmid, IPC_RMID, NULL);
};

} 
#endif