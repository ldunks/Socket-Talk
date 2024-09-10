// write.h
// Liam Duncan

#ifndef _WRITE_H_
#define _WRITE_H_


#include "list.h"
#define MAX_LEN 512

// used to take in message from user and add to end of list
// checks if there are any nodes available, if not it waits until there are
// once available, it adds the node to the list holding sending messages
void* Write_Message(void* unused);

// used to alert the send.c that there is a message that needs to be sent
void Write_Signal();

// used to create thread and set list that will store messages to send
void Write_Init(List* list_send);


// used to cancel the thread and destory the mutex, as well as free the list
void Write_Shutdown();

#endif