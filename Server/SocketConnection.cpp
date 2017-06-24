//
// Created by user on 27/04/2017.
//

#include <iostream>
#include "SocketConnection.h"

using namespace std;

bool check_connection_result(int result) {
    int err;
    if (0 < result) {
        return true;
    }
    if (0 == result) {
        return false;
    }
    err = WSAGetLastError();
    if (WSAECONNRESET != err) {
        throw "communication failed with error: " + err;
    }
    return false;
}

int SocketConnection::send_buffer(char *buffer, size_t buf_size) {
    int result;

    result = send(conn_socket, (char *)&buf_size, sizeof(buf_size), 0);
    if (!check_connection_result(result)) {
        return -1;
    }

    result = send(conn_socket, buffer, buf_size, 0);
    if (!check_connection_result(result)) {
        return -1;
    }
    return result;
}


int SocketConnection::recv_data(char *recvbuf, size_t max_bufsize) {
    int result;
    int read_bytes = 0;
    size_t buf_size = 0;

    result = recv(conn_socket, (char *)&buf_size, sizeof(buf_size), 0);
    if (!check_connection_result(result)) {
        return -1;
    }

    if ((result != sizeof(buf_size)) || (buf_size > max_bufsize)) {
        throw "invalid message";
    }
//    cout << "Should receive " << buf_size << " bytes" << endl;

    // Receive until the peer closes the connection
    while(buf_size > read_bytes) {
        result = recv(conn_socket, recvbuf + read_bytes, buf_size - read_bytes, 0);
        if (!check_connection_result(result)) {
            return -1;
        }
        read_bytes += result;
    }

    return buf_size;
}
