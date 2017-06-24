//
// Created by user on 27/04/2017.
//

#include <iostream>
#include "ClientConnection.h"


ClientConnection::ClientConnection(string host_name, string port_name) {
    WSADATA wsaData;

    // Initialize Winsock
    int socket_res = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (0 != socket_res) {
        throw "WSAStartup failed with error: " + socket_res;
    }

    connect_socket(host_name, port_name);
}

ClientConnection::~ClientConnection() {
    if (INVALID_SOCKET != conn_socket) {
        // shutdown the connection since we're done
        if (SOCKET_ERROR == shutdown(conn_socket, SD_SEND)) {
            cout << "shutdown failed with error: " << WSAGetLastError() << endl;
        }
        closesocket(conn_socket);
        conn_socket = INVALID_SOCKET;
    }
    WSACleanup();
}

void ClientConnection::connect_socket(string hostname, string port_name) {
    int socket_res;
    struct addrinfo *result = NULL, *ptr = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    socket_res = getaddrinfo(hostname.c_str(), port_name.c_str(), &hints, &result);
    if (0 != socket_res) {
        throw "getaddrinfo failed with error: " + socket_res;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; NULL != ptr; ptr=ptr->ai_next) {
        // Create a SOCKET for connecting to server
        conn_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (INVALID_SOCKET == conn_socket) {
            throw "socket failed with error: " + WSAGetLastError();
        }

        // Connect to server.
        socket_res = connect(conn_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (SOCKET_ERROR == socket_res) {
            closesocket(conn_socket);
            conn_socket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (INVALID_SOCKET == conn_socket) {
        throw "Unable to connect to server!";
    }
}

#define MAX_BUFFER_SIZE (1024)

void ClientConnection::send_and_receive(TunnelRequest *cmd) {
    char buf[MAX_BUFFER_SIZE];

    int buf_size = cmd->serialize((byte_t*)buf, MAX_BUFFER_SIZE);
    int written_bytes = send_buffer(buf, buf_size);
    cout << "Written bytes: " << written_bytes << endl;
    buf_size = recv_data(buf, (size_t)MAX_BUFFER_SIZE);
    TunnelResponse *res = parse_response((byte_t*)buf, (size_t)buf_size);
    cout << res->get_type() << endl;
};
