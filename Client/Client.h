//
// Created by user on 27/04/2017.
//

#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H


#include "SocketConnection.h"

class Client: public SocketConnection {
private:
    int connect_socket(const char *host_name, const char *port_name);
public:
    Client(const char *host_name, const char *port_name);
    ~Client();
};


#endif //CLIENT_CLIENT_H
