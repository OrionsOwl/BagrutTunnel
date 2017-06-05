//
// Created by user on 27/04/2017.
//

#include <cstdio>
#include "SocketConnection.h"

int SocketConnection::send_buffer(char *buffer, uint16_t buf_size) {
    int result;

    result = send(conn_socket, (char *)&buf_size, sizeof(buf_size), 0);
    if (SOCKET_ERROR == result) {
        printf("send failed with error: %d\n", WSAGetLastError());
        return -1;
    }

    result = send(conn_socket, buffer, buf_size, 0);
    if (SOCKET_ERROR == result) {
        printf("send failed with error: %d\n", WSAGetLastError());
        return -1;
    }
    return result;
}


int SocketConnection::recv_data(char *recvbuf, int max_bufsize, int *recvsize) {
    int result;
    int read_bytes = 0;
    uint16_t buf_size = 0;

    result = recv(conn_socket, (char *)&buf_size, sizeof(buf_size), 0);
    // Connection closed
    if (0 == result) {
        return -2;
    }
    if (result < 0) {
        printf("recv failed with error: %d\n", WSAGetLastError());
        return -1;
    }
    if ((result != sizeof(buf_size)) || (buf_size > max_bufsize)) {
        printf("Invalid message\n");
        return -3;
    }
//    printf("Should receive %d bytes\n", buf_size);

    // Receive until the peer closes the connection
    while(buf_size > read_bytes) {
        result = recv(conn_socket, recvbuf + read_bytes, buf_size - read_bytes, 0);
        // Connection closed
        if (0 == result) {
            return -2;
        }
        if (0 > result) {
            printf("recv failed with error: %d\n", WSAGetLastError());
            return -1;
        }
        read_bytes += result;
    }

    *recvsize = buf_size;
    return 0;
}
