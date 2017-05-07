//
// Created by tal on 04-May-17.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include "SocketConnection.h"

class Server : public SocketConnection {
private:
    int get_connection(const char *host_name, const char *port_name, int buflen);

public:
    Server(const char *host_name, const char *port_name, int buflen);
    SOCKET get_conn_socket();
    ~Server();
};


#endif //SERVER_SERVER_H
