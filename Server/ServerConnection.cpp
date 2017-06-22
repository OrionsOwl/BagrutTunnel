//
// Created by user on 27/04/2017.
//

#include <iostream>
#include "ServerConnection.h"

using namespace std;

ServerConnection::ServerConnection(const char *port_name) {
    int socket_res;
    WSADATA wsaData;

    // Initialize Winsock
    socket_res = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (0 != socket_res) {
        throw "WSAStartup failed with error: " + socket_res;
    }

    initiate_listening_socket(port_name);
    cout << "Listening socket initiated" << endl;
}


ServerConnection::~ServerConnection() {
    if (INVALID_SOCKET != listen_socket) {
        closesocket(listen_socket);
        listen_socket = INVALID_SOCKET;
    }
    close_client();

    WSACleanup();
}

void ServerConnection::initiate_listening_socket(const char *port_name) {
//    WSADATA wsaData;
    int socket_res;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    // Initialize Winsock
//    socket_res = WSAStartup(MAKEWORD(2,2), &wsaData);
//    if (socket_res != 0) {
//        cout << "WSAStartup failed with error: " << socket_res << endl;
//        return 1;
//    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    socket_res = getaddrinfo(NULL, port_name, &hints, &result);
    if (0 != socket_res) {
        throw "getaddrinfo failed with error: " + socket_res;
    }

    // Create a SOCKET for connecting to server
    listen_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listen_socket == INVALID_SOCKET) {
        freeaddrinfo(result);
        throw "socket failed with error: " + WSAGetLastError();
    }

    // Setup the TCP listening socket
    socket_res = bind(listen_socket, result->ai_addr, (int)result->ai_addrlen);
    if (socket_res == SOCKET_ERROR) {
        freeaddrinfo(result);
        throw "bind failed with error: " + WSAGetLastError();
    }

    freeaddrinfo(result);

    if (SOCKET_ERROR == listen(listen_socket, SOMAXCONN)) {
//        throw "Couldn't initiate listening socket!";
        throw "listen failed with error: " + WSAGetLastError();
    }
}

void ServerConnection::wait_for_client() {
    SOCKADDR_IN client_info = {0};
    int addrsize = sizeof(client_info);

    cout << "Waiting for client: ";
    // Accept a client socket
    conn_socket = accept(listen_socket, (struct sockaddr*)&client_info, &addrsize);
    if (conn_socket == INVALID_SOCKET) {
        throw "accept failed with error: " + WSAGetLastError();
    }
    cout << "Client connected <" << inet_ntoa(client_info.sin_addr) << ">" << endl;
}

void ServerConnection::close_client() {
    if (INVALID_SOCKET == conn_socket) {
        return;
    }
    // shutdown the connection since we're done
    if (SOCKET_ERROR == shutdown(conn_socket, SD_SEND)) {
        cout << "shutdown failed with error: " << WSAGetLastError() << endl;
    }
    closesocket(conn_socket);
    conn_socket = INVALID_SOCKET;
}