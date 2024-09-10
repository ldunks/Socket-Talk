// send.h
// Liam Duncan

#ifndef _SEND_H_
#define _SEND_H_

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "list.h"


// used to actually know that message is comimg from write and send to friend
// if success then the node is removed from the list
void* Send_Message(void* unused);

// used to create thread, set the list that will store sending messages, and friend socket
void Send_Init(List* list_send, struct addrinfo** friendAddress);

// used to shut down the threads
void Send_Shutdown();

#endif