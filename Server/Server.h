//
// Created by user on 27/04/2017.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H


#include "SocketConnection.h"

class Server: public SocketConnection {
private:
    SOCKET listen_socket = INVALID_SOCKET;
    int initiate_listening_socket(const char *port_name);
public:
    Server(const char *port_name);
    ~Server();

    int wait_for_client();
    int close_client();
};


#endif //SERVER_SERVER_H
