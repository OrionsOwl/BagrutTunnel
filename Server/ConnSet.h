//
// Created by user on 09/06/2017.
//

#ifndef SERVER_CONNECTION_HANDLERS_H
#define SERVER_CONNECTION_HANDLERS_H

#include <set>
#include <string>

#include "TunnelRequest.h"
#include "ConnHandler.h"

using namespace std;

typedef set<ConnHandler>::iterator conn_set_iter;

class ConnSet: public set<ConnHandler> {
private:
    conn_set_iter _find_entry(ConnHandler conn) { return find(conn); }
public:
    void add(ConnHandler conn);
    void remove(ConnHandler conn);
    ConnHandler get_entry(ConnHandler conn);

    void add(ComputerID comm) { add(ConnHandler(comm)); }
    void remove(ComputerID comm) { remove(ConnHandler(comm)); }
    ConnHandler get_entry(ComputerID comm) { return get_entry(ConnHandler(comm)); }

//    void add(ComputerID comm) { ConnHandler con(comm); add(con); }
//    void remove(ComputerID comm) { ConnHandler con(comm); remove(con); }
//    ConnHandler get_entry(ComputerID comm) { ConnHandler con(comm); get_entry(con); }

    friend ostream& operator<<(ostream& os, const ConnSet& ch);
};


#endif // SERVER_CONNECTION_HANDLERS_H
