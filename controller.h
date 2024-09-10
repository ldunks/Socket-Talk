// controller.h
// Assignment 2 - CMPT 300
// Liam Duncan - 301476562
// March 1, 2024


#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "list.h"




// function used  to create a socket
// will return an int socketdescriptor so it can be used later
// if socket already exists then it returns the socketDescriptor for that socket
int Controller_CreateSocket();

// function used to remove node from list
// once node is removed it signals that there is a node available if a function is waiting
void Controller_ListRemoveNode(List* list_to_remove);

// function used to add node to list
// if successfully added node then 1 is returned
// otherwise -1 is returned telling t he caller that it needs
// to wait for a new node to be available.
int Controller_ListAddNode(List* list_to_add, void* message);

// function used to check wait until there is a node available
// Once a node is removed using Controller_ListRemoveNode() then
// then alerts to that there is a node available to use.
void Controller_CheckAvailNode();

// basic function used to free messages (items) from the list
void Controller_FreeItem(void* Item);

// this is used to alert the controller that it is ok to kill program
//
void Controller_AlertKillProgram();

// Function used to init the prorgam. Takes in the home port,
// a pointer to the list used to send messages, a pointer to the
// list used to receive messages and the info of the freindaddress
void Controller_Init(int home_port, List* list_send, List* list_received, struct addrinfo** friendAddress);

// Function used to shutdown read, write, send, and receive, as
// well as destroy mutexes etc. 
void Controller_KillProgram();

#endif