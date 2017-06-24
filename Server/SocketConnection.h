//
// Created by user on 27/04/2017.
//

#ifndef _SOCKETCONNECTION_H
#define _SOCKETCONNECTION_H

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <cstdint>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


class SocketConnection {
protected:
    SOCKET conn_socket = INVALID_SOCKET;
public:
    int send_buffer(char *buffer, size_t buf_size);
    int recv_data(char *recvbuf, size_t max_bufsize);
};


#endif //_SOCKETCONNECTION_H
