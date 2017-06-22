//
// Created by user on 27/04/2017.
//

#include <iostream>
#include "SocketConnection.h"

using namespace std;

int SocketConnection::send_buffer(char *buffer, size_t buf_size) {
    int result;

    if (SOCKET_ERROR == send(conn_socket, (char *)&buf_size, sizeof(buf_size), 0)) {
        throw "send failed with error: " + WSAGetLastError();
    }

    result = send(conn_socket, buffer, buf_size, 0);
    if (SOCKET_ERROR == result) {
        throw "send failed with error: " + WSAGetLastError();
    }
    return result;
}


int SocketConnection::recv_data(char *recvbuf, size_t max_bufsize) {
    int result;
    int read_bytes = 0;
    uint16_t buf_size = 0;

    result = recv(conn_socket, (char *)&buf_size, sizeof(buf_size), 0);
    // Connection closed
    if (0 == result) {
        return -1;
    }
    if (0 > result) {
        throw "recv failed with error: " + WSAGetLastError();
    }
    if ((result != sizeof(buf_size)) || (buf_size > max_bufsize)) {
        throw "invalid message";
    }
//    cout <<Trissmitp!
// "Should receive " << buf_size << " bytes" << endl;

    // Receive until the peer closes the connection
    while(buf_size > read_bytes) {
        result = recv(conn_socket, recvbuf + read_bytes, buf_size - read_bytes, 0);
        // Connection closed
        if (0 == result) {
            return -1;
        }
        if (0 > result) {
            throw "recv failed with error: " + WSAGetLastError();
        }
        read_bytes += result;
    }

    return buf_size;
}
