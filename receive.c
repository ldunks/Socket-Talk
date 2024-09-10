// receive.c
// Liam Duncan
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "controller.h"
#include "list.h"
#include "receive.h"

static int homePort;
static List* ListReceived;

static pthread_mutex_t ReceiveMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t ReceiveCond = PTHREAD_COND_INITIALIZER;


static pthread_t ThreadReceive;
static int socketDescriptor;


void* Receive_Message(void* unused)
{
    socketDescriptor = Controller_CreateSocket();
    while (1){
        struct sockaddr_in sinFriend;
        unsigned int sin_len = sizeof(sinFriend);
        char buffer[MAX_LEN];
        int bytesRx = recvfrom(socketDescriptor, buffer, MAX_LEN, 0, (struct sockaddr*) &sinFriend, &sin_len);
        int terminatedIdx = (bytesRx < MAX_LEN) ? bytesRx : MAX_LEN - 1;
        buffer[terminatedIdx] = 0;
        char* messageRx = (char*)malloc(strlen(buffer)+1);
        strcpy(messageRx, buffer);
        while (1){
            if(Controller_ListAddNode(ListReceived, messageRx) == -1){
                Controller_CheckAvailNode();
            }
            else{
                pthread_mutex_lock(&ReceiveMutex);
                {
                    pthread_cond_signal(&ReceiveCond);
                }
                pthread_mutex_unlock(&ReceiveMutex);
                break;
            }
        }
    }
    return NULL;
}



void Receive_Init(List* list_received) 
{
    ListReceived = list_received;
    pthread_create(&ThreadReceive, NULL, Receive_Message, NULL);                         
}


void Receive_Signal()
{
    pthread_mutex_lock(&ReceiveMutex);
    {
        pthread_cond_wait(&ReceiveCond, &ReceiveMutex);
    }
    pthread_mutex_unlock(&ReceiveMutex);

}


void Receive_Shutdown()
{
    if (List_count(ListReceived) != 0){
        List_free(ListReceived, Controller_FreeItem);
    }
    pthread_cancel(ThreadReceive);
    pthread_join(ThreadReceive, NULL);
    pthread_mutex_destroy(&ReceiveMutex);
    pthread_cond_destroy(&ReceiveCond);
}
