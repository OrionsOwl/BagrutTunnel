//
// Created by user on 27/04/2017.
//

#ifndef SERVER_SERVER_CONNECTION_H
#define SERVER_SERVER_CONNECTION_H


#include "SocketConnection.h"

class ServerConnection: public SocketConnection {
private:
    SOCKET listen_socket = INVALID_SOCKET;
    void initiate_listening_socket(const char *port_name);
public:
    ServerConnection(const char *port_name);
    ~ServerConnection();

    void wait_for_client();
    void close_client();
};


#endif //SERVER_SERVER_CONNECTION_H
