//
// Created by user on 09/06/2017.
//

#ifndef SERVER_CONNECTION_HANDLERS_H
#define SERVER_CONNECTION_HANDLERS_H

#include <set>
#include <string>

#include "TunnelCommand.h"
#include "ConnectionHandler.h"

using namespace std;

class ConnectionHandlers: set<ConnectionHandler> {
private:
    set<ConnectionHandler>::iterator _find_entry(ConnectionHandler conn) { return find(conn); }
//    set<ConnectionHandler>::iterator _find_entry(string host, int ifs);
public:
    void add(ConnectionHandler conn);
    void remove(ConnectionHandler conn);
    ConnectionHandler get_entry(ConnectionHandler conn);

//    void add(string host, int ifs) { add(ConnectionHandler(host, ifs)); }
//    void remove(string host, int ifs) { remove(ConnectionHandler(host, ifs)); }
//    ConnectionHandler get_entry(string host, int ifs) { return get_entry(ConnectionHandler(host, ifs)); }

    void add(ComputerID *comm) { add(ConnectionHandler(comm)); }
    void remove(ComputerID *comm) { remove(ConnectionHandler(comm)); }
    ConnectionHandler get_entry(ComputerID *comm) { return get_entry(ConnectionHandler(comm)); }

    friend ostream& operator<<(ostream& os, const ConnectionHandlers& ch);
};

typedef set<ConnectionHandler>::iterator connection_handlers_iter;


#endif // SERVER_CONNECTION_HANDLERS_H
