//
// Created by user on 15/06/2017.
//

#ifndef SERVER_CONNECTION_HANDLER_H
#define SERVER_CONNECTION_HANDLER_H

#include <string>
#include "TunnelCommand.h"

using namespace std;

class ConnectionHandler {
private:
    ComputerID *comm;
public:
    ConnectionHandler(ComputerID *comm, bool should_connect): comm(comm) {
        if (should_connect) {
            connect();
        }
    }
    ConnectionHandler(ComputerID *comm): ConnectionHandler(comm, false) {};
    ~ConnectionHandler();
    void connect();
    string communicate(string command);
    ComputerID* get_id() { return comm; }
    bool operator< (const ConnectionHandler& other) const;
    bool operator== (const ConnectionHandler &other);
    bool operator!= (const ConnectionHandler &other);
    friend ostream& operator<<(ostream& os, const ConnectionHandler& other);
};


#endif //SERVER_CONNECTION_HANDLER_H
