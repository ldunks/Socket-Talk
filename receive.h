// receive.h
// Assignment 2 - CMPT 300
// Liam Duncan - 301476562
// March 1, 2024

#ifndef _RECEIVE_H_
#define _RECEIVE_H_

#include "list.h"

#define MAX_LEN 512 


// handles the actual message that is being received
// checks if there are any nodes available, if not it waits until there are
// once available, it adds the node to the list holding received messages
void* Receive_Message(void* unused);

// used to create thread and set the list that will store received messages
void Receive_Init(List* list_received);

// used to alert the read.c that there is message to be read
void Receive_Signal();

// used to shut down threads and mutex as well as free the list if needed
void Receive_Shutdown();

#endif