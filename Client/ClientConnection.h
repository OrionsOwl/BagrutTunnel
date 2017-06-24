//
// Created by user on 27/04/2017.
//

#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include <string>
#include "SocketConnection.h"
#include "TunnelRequest.h"
#include "TunnelResponse.h"


using namespace std;


class ClientConnection: public SocketConnection {
private:
    void connect_socket(string host_name, string port_name);
public:
    ClientConnection(string host_name, string port_name);
    ~ClientConnection();
    void send_and_receive(TunnelRequest *cmd);
};


#endif //CLIENT_CLIENT_H
