// controller.c
// Assignment 2 - CMPT 300
// Liam Duncan - 301476562
// March 1, 2024

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include "list.h"
#include "write.h"
#include "read.h"
#include "send.h"
#include "receive.h"
#include "controller.h"


static int homePort;
static int socketDescriptor;

static pthread_mutex_t ControllerMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t ControllerCond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t ListAddMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t ListRemoveMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t NodeAvailMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t NodeAvailCond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t SocketMutex = PTHREAD_MUTEX_INITIALIZER;

struct sockaddr_in sinCreate;

static int socketExist = -1;



int Controller_CreateSocket()
{
    pthread_mutex_lock(&SocketMutex);
    {
        if (socketExist == -1){
        memset(&sinCreate, 0, sizeof(sinCreate));
        sinCreate.sin_family = AF_INET;
        sinCreate.sin_addr.s_addr = htonl(INADDR_ANY);
        sinCreate.sin_port = htons(homePort);
        socketDescriptor= socket(PF_INET, SOCK_DGRAM, 0);

        if(bind(socketDescriptor, (struct sockaddr*) &sinCreate, sizeof(sinCreate)) != 0){
            printf("Failed to bind socket\n");
            Controller_KillProgram();
        }
            socketExist = 1;
        }
    }
    pthread_mutex_unlock(&SocketMutex);
    return socketDescriptor;
    
}



void Controller_ListRemoveNode(List* list_to_remove)
{
    pthread_mutex_lock(&ListRemoveMutex);
    {
        List_remove(list_to_remove);
        pthread_cond_signal(&NodeAvailCond);
    }
    pthread_mutex_unlock(&ListRemoveMutex);

}

int Controller_ListAddNode(List* list_to_add, void* message)
{
    int val = 1;
    pthread_mutex_lock(&NodeAvailMutex);
    {
        if (List_append(list_to_add, message) == -1){
            val = -1;
        }
    }
    pthread_mutex_unlock(&NodeAvailMutex);
    return val;
}


void Controller_CheckAvailNode()
{
    pthread_mutex_lock(&NodeAvailMutex);
    {
        pthread_cond_wait(&NodeAvailCond, &NodeAvailMutex);
    }
    pthread_mutex_unlock(&NodeAvailMutex);
}

void Controller_FreeItem(void* Item)
{
    if (Item){
        free(Item);
    }

}


void Controller_AlertKillProgram()
{
    pthread_mutex_lock(&ControllerMutex);
    {
        pthread_cond_signal(&ControllerCond);
    }
    pthread_mutex_unlock(&ControllerMutex);

}


void Controller_Init(int home_port, List* list_send, List* list_received, struct addrinfo** friendAddress)
{
    homePort = home_port;
    Send_Init(list_send, friendAddress);
    Receive_Init(list_received);
    Write_Init(list_send);
    Read_Init(list_received);
}

void Controller_KillProgram()
{
    pthread_mutex_lock(&ControllerMutex);
    {
        pthread_cond_wait(&ControllerCond, &ControllerMutex);

    }
    pthread_mutex_unlock(&ControllerMutex);
    printf("Killing program\n");

    Send_Shutdown();
    Write_Shutdown();
    Read_Shutdown();
    Receive_Shutdown();
    pthread_cond_destroy(&ControllerCond);
    pthread_mutex_destroy(&ControllerMutex);
    pthread_mutex_destroy(&ListAddMutex);
    pthread_mutex_destroy(&ListRemoveMutex);
    pthread_mutex_destroy(&NodeAvailMutex);
    pthread_cond_destroy(&NodeAvailCond);
    pthread_mutex_destroy(&SocketMutex);
    
}
