// main.c
// Liam Duncan
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <string.h>
#include "list.h"
#include "controller.h"


int main(int argc, char* argv[]) {

	if (argc != 4) {
		printf("Fail: incorrect arguments\n");
		return 0;
	}

	printf("S-talk\n Type '!' to close the program\n\n");
	struct addrinfo *friendAddress;
	int addr_check;
	struct  addrinfo hint;
	memset(&hint, 0, sizeof(struct addrinfo));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_DGRAM;
	addr_check = getaddrinfo(argv[2], argv[3], &hint, &friendAddress);
	if (addr_check != 0) {
		printf("Fail: address check\n");
		return 0;
	}

	List* listReceived = List_create();
	List* listSend = List_create();

	if (listReceived == NULL || listSend == NULL) {
		printf("Fail: Unable to create one of the required list.\n");
		return 0;
	}

	Controller_Init(atoi(argv[1]), listSend, listReceived, &friendAddress);
	Controller_KillProgram();
	return 0;
}
