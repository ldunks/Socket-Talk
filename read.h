// read.h
// Assignment 2 - CMPT 300
// Liam Duncan - 301476562
// March 1, 2024

#ifndef _READ_H_
#define _READ_H_

#include "list.h"

// actually prints the message and checks if it is "!"
// if the message is read successfully then the node is removed from the list
void* Read_Print(void* unused);

// used to create thread and set the list that will store received messages
void Read_Init(List* list_received);


// used to shut down threads
void Read_Shutdown();

#endif