#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include "GameState.h"

struct client{ //functions as client or server
    WSADATA wsaData;
    int iResult;

	SOCKET ListenSocket;
    SOCKET ClientSocket;

    struct addrinfo *result,
                    *ptr,
                    hints;
    char* sendbuf;
	int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen;
};