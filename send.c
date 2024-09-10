// send.c
// Liam Duncan
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#include "send.h"
#include "write.h"
#include "list.h"
#include "controller.h"

static pthread_t ThreadSend;
static List* listSend;
struct addrinfo* sinFriend;
static int socketDescriptor;


void* Send_Message(void* unused)
{
    while(1){
        Write_Signal();
        socketDescriptor = Controller_CreateSocket();
        char* sendMessage = List_first(listSend);
        if(sendto(socketDescriptor, sendMessage, strlen(sendMessage)+1, 0, sinFriend->ai_addr, sinFriend->ai_addrlen) == 1){
            printf("Fail: unable to send message\n");
        }
        if (sendMessage[0]== '!' && sendMessage[2] == 0) { 
			free(sendMessage);
            Controller_AlertKillProgram();
		}
		else {
			free(sendMessage);
			Controller_ListRemoveNode(listSend);
		}
    }
}


void Send_Init(List* list_send, struct addrinfo** friendAddress)
{
    listSend = list_send;
    sinFriend = *friendAddress;
    pthread_create(&ThreadSend, NULL, Send_Message, NULL);
}

void Send_Shutdown()
{
    pthread_cancel(ThreadSend);
    pthread_join(ThreadSend, NULL);
    free(sinFriend);
}
