//
// Created by tal on 04-May-17.
//

#include <cstdio>
#include "Server.h"

Server::Server(const char *host_name, const char *port_name, int buflen) {
    int socket_res;
    WSADATA wsaData;

    // Initialize Winsock
    socket_res = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (0 != socket_res) {
        printf("WSAStartup failed with error: %d\n", socket_res);
        /* raise Exception */
    }

    get_connection(host_name, port_name, buflen);
}

int Server::get_connection(const char *host_name, const char *port_name, int buflen) {
    int socket_res;
   // SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    //int iSendResult;
    //char recvbuf[buflen];
    //int recvbuflen = buflen;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    socket_res = getaddrinfo(NULL, port_name, &hints, &result);
    if (0 != socket_res) {
        printf("getaddrinfo failed with error: %d\n", socket_res);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    conn_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (conn_socket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    socket_res = bind( conn_socket, result->ai_addr, (int)result->ai_addrlen);
    if (socket_res == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(conn_socket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    socket_res = listen(conn_socket, SOMAXCONN);
    if (socket_res == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(conn_socket);
        WSACleanup();
        return 1;
    }

     //Accept a client socket
    ClientSocket = accept(conn_socket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(conn_socket);
        WSACleanup();
        return 1;
    }

   // // Echo the buffer back to the sender
   //         iSendResult = send_buffer(conn_socket, recvbuf, socket_res, 0 );
   //         if (iSendResult == SOCKET_ERROR) {
   //             printf("send failed with error: %d\n", WSAGetLastError());
   //             closesocket(ClientSocket);
    //            WSACleanup();
   //             return 1;
   //         }
   //         printf("Bytes sent: %d\n", iSendResult);
    return 0;
}

SOCKET Server::get_conn_socket() {
    return conn_socket;
}

Server::~Server() {
    closesocket(conn_socket);
    WSACleanup();
}



