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
    struct bitsForSending sendbuf;
	int iSendResult;
    struct bitsForSending recvbuf;
    int recvbuflen;
};