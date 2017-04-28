#define WIN32_LEAN_AND_MEAN

//#include <windows.h>
//#include <winsock2.h>
//#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "Client.h"


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
//#pragma comment (lib, "Ws2_32.lib")
//#pragma comment (lib, "Mswsock.lib")
//#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
#define DEFAULT_HOST "localhost"

int __cdecl main(int argc, char **argv)
{
    Client conn(DEFAULT_HOST, DEFAULT_PORT);
    char *sendbuf = (char *) "this is a test";
    int res;
//    char recvbuf[DEFAULT_BUFLEN];
//    int recvbuflen = DEFAULT_BUFLEN;

    // Validate the parameters
//    if (argc != 2) {
//        printf("usage: %s server-name\n", argv[0]);
//        return 1;
//    }

    // Send an initial buffer
    res = conn.send_buffer(sendbuf, (uint16_t)strlen(sendbuf));
    if (-1 == res) {
        return 1;
    }

    printf("Bytes Sent: %d\n", res);

    res = conn.recv_data();
//    if (-1 == res) {
//        return 1;
//    }

//    printf("Bytes Sent: %d\n", res);

    // shutdown the connection since no more data will be sent
//    iResult = shutdown(ConnectSocket, SD_SEND);
//    if (iResult == SOCKET_ERROR) {
//        printf("shutdown failed with error: %d\n", WSAGetLastError());
//        closesocket(ConnectSocket);
//        WSACleanup();
//        return 1;
//    }

    // Receive until the peer closes the connection
//    do {
//
//        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
//        if ( iResult > 0 )
//            printf("Bytes received: %d\n", iResult);
//        else if ( iResult == 0 )
//            printf("Connection closed\n");
//        else
//            printf("recv failed with error: %d\n", WSAGetLastError());
//
//    } while( iResult > 0 );

    return 0;
}