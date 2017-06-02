//
// Created by tal on 04-May-17.
//

#ifndef SERVER_SOCKETCONNECTION_H
#define SERVER_SOCKETCONNECTION_H

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <cstdint>
//#include <stdlib.h>
//#include <stdio.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
//#pragma comment (lib, "Ws2_32.lib")
//#pragma comment (lib, "Mswsock.lib")
//#pragma comment (lib, "AdvApi32.lib")
// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")

class SocketConnection {
protected:
    SOCKET conn_socket = INVALID_SOCKET;
public:
    int send_buffer(char *buffer, uint16_t buf_size);
    int recv_data();
};


#endif //SERVER_SOCKETCONNECTION_H