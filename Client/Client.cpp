//
// Created by user on 27/04/2017.
//

#include <cstdio>
#include "Client.h"

Client::Client(const char *host_name, const char *port_name) {
    int socket_res;
    WSADATA wsaData;

    // Initialize Winsock
    socket_res = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (0 != socket_res) {
        printf("WSAStartup failed with error: %d\n", socket_res);
        /* raise Exception */
    }

    connect_socket(host_name, port_name);
}

int Client::connect_socket(const char *hostname, const char *port_name) {
    int socket_res;
    struct addrinfo *result = NULL, *ptr = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    socket_res = getaddrinfo(hostname, port_name, &hints, &result);
    if (0 != socket_res) {
        printf("getaddrinfo failed with error: %d\n", socket_res);
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; NULL != ptr; ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        conn_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (INVALID_SOCKET == conn_socket) {
            printf("socket failed with error: %d\n", WSAGetLastError());
            return 1;
        }

        // Connect to server.
        socket_res = connect( conn_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (SOCKET_ERROR == socket_res) {
            closesocket(conn_socket);
            conn_socket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (INVALID_SOCKET == conn_socket) {
        printf("Unable to connect to server!\n");
        return 1;
    }
    return 0;
}

Client::~Client() {
    closesocket(conn_socket);
    WSACleanup();
}
