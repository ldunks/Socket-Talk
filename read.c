// read.c
// Liam Duncan

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include "receive.h"
#include "list.h"
#include "controller.h"
#include "read.h"


static pthread_t ThreadRead;
List* listReceived;


void* Read_Print(void* unused)
{
    while (1){
        Receive_Signal();
        char* messageRx = List_first(listReceived);
        printf("New Message: %s\n",  messageRx);
        fflush(stdout); 
        if (messageRx[0] == '!' && messageRx[2] == 0) {
            free(messageRx); 
            fputs("Remote terminal killed program\n", stdout);
            fflush(stdout); 
            Controller_AlertKillProgram();
        }
        else{
            free(messageRx);
            Controller_ListRemoveNode(listReceived);
        }
    }
}

void Read_Init(List* list_received)
{
    listReceived = list_received;
    pthread_create(&ThreadRead, NULL, Read_Print, NULL);
}

void Read_Shutdown()
{
    pthread_cancel(ThreadRead);
    pthread_join(ThreadRead, NULL);
}
