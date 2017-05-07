//
// Created by tal on 04-May-17.
//

#include <cstdio>
#include "SocketConnection.h"

int SocketConnection::send_buffer(char *buffer, uint16_t buf_size) {
    int result;

//    result = send(conn_socket, (const char *) buf_size, sizeof(buf_size), 0);
//    if (SOCKET_ERROR == result) {
//        printf("send failed with error: %d\n", WSAGetLastError());
//        return -1;
//    }

    result = send(conn_socket, buffer, buf_size, 0);
    if (SOCKET_ERROR == result) {
        printf("send failed with error: %d\n", WSAGetLastError());
        return -1;
    }
    return result;
}

#define DEFAULT_BUFLEN 512

int SocketConnection::recv_data() {
    int result;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    // Receive until the peer closes the connection
    do {

        result = recv(conn_socket, recvbuf, recvbuflen, 0);
        if (0 < result)
            printf("Bytes received: %d\n", result);
        else if (0 == result)
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while(0 < result);

    // No longer need server socket
   // closesocket(conn_socket);

    return 0;
}
