// write.c
// Assignment 2 - CMPT 300
// Liam Duncan - 301476562
// March 1, 2024

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>

#include "controller.h"
#include "list.h"
#include "write.h"

List* ListSend;
static pthread_t ThreadWrite;
static pthread_mutex_t WriteMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t WriteCond = PTHREAD_COND_INITIALIZER;



void* Write_Message(void* unused)
{
    while (1){
        char buffer[MAX_LEN];
        fgets(buffer, MAX_LEN-1, stdin);
        char* messageWrite =  (char*)malloc(strlen(buffer)+1);
        strcpy(messageWrite, buffer);
        while (1){
            if (Controller_ListAddNode(ListSend, messageWrite) == -1){
                Controller_CheckAvailNode();
            }
            else{
                pthread_mutex_lock(&WriteMutex);
                {
                    pthread_cond_signal(&WriteCond);
                }
                pthread_mutex_unlock(&WriteMutex);
                break;
            }
        }
    }
}

void Write_Signal()
{
    pthread_mutex_lock(&WriteMutex);
    {
        pthread_cond_wait(&WriteCond, &WriteMutex);
    }
    pthread_mutex_unlock(&WriteMutex);
}

void Write_Init(List* list_send)
{
    ListSend = list_send;
    pthread_create(&ThreadWrite, NULL, Write_Message, NULL);
}




void Write_Shutdown()
{
    if (List_count(ListSend) != 0){
        List_free(ListSend, Controller_FreeItem);
    }
    pthread_cancel(ThreadWrite);
    pthread_join(ThreadWrite, NULL);
    pthread_mutex_destroy(&WriteMutex);
    pthread_cond_destroy(&WriteCond);

}
