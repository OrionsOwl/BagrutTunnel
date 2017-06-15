//
// Created by user on 27/04/2017.
//

#include <cstdio>
#include "Server.h"

Server::Server(const char *port_name) {
    int socket_res;
    WSADATA wsaData;

    // Initialize Winsock
    socket_res = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (0 != socket_res) {
        printf("WSAStartup failed with error: %d\n", socket_res);
        /* raise Exception */
    }

    if (0 != initiate_listening_socket(port_name)) {
        printf("Couldn't initiate listening socket!\n");
    } else {
        printf("Listening socket initiated\n");
    }
}


Server::~Server() {
    if (INVALID_SOCKET != listen_socket) {
       closesocket(listen_socket);
        listen_socket = INVALID_SOCKET;
    }
    close_client();

    WSACleanup();
}

int Server::initiate_listening_socket(const char *port_name) {
    WSADATA wsaData;
    int socket_res;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    // Initialize Winsock
    socket_res = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (socket_res != 0) {
        printf("WSAStartup failed with error: %d\n", socket_res);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    socket_res = getaddrinfo(NULL, port_name, &hints, &result);
    if (socket_res != 0) {
        printf("getaddrinfo failed with error: %d\n", socket_res);
        return 1;
    }

    // Create a SOCKET for connecting to server
    listen_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listen_socket == INVALID_SOCKET) {
        printf("socket failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        return 1;
    }

    // Setup the TCP listening socket
    socket_res = bind(listen_socket, result->ai_addr, (int)result->ai_addrlen);
    if (socket_res == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        return 1;
    }

    freeaddrinfo(result);

    socket_res = listen(listen_socket, SOMAXCONN);
    if (socket_res == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        return 1;
    }

    return 0;
}

int Server::wait_for_client() {
    SOCKADDR_IN client_info = {0};
    int addrsize = sizeof(client_info);

    printf("Waiting for client: ");
    // Accept a client socket
    conn_socket = accept(listen_socket, (struct sockaddr*)&client_info, &addrsize);
    if (conn_socket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Client connected <%s>\n", inet_ntoa(client_info.sin_addr));

    return 0;
}

int Server::close_client() {
    int socket_res;

    if (INVALID_SOCKET != conn_socket) {
        // shutdown the connection since we're done
        socket_res = shutdown(conn_socket, SD_SEND);
        if (socket_res == SOCKET_ERROR) {
            printf("shutdown failed with error: %d\n", WSAGetLastError());
        }
        closesocket(conn_socket);
        conn_socket = INVALID_SOCKET;
    }
}