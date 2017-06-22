//
// Created by user on 09/06/2017.
//

#include "ConnectionHandlers.h"
#include <iostream>

using namespace std;

//set<ConnectionHandler>::iterator ConnectionHandlers::_find_entry(string host, int ifs) {
//    return _find_entry(ConnectionHandler(new ComputerID(host, ifs)));
//}

void ConnectionHandlers::add(ConnectionHandler conn) {
    if (_find_entry(conn) != end()) {
        throw "Entry already exists";
    }
    conn.connect();
    insert(conn);
}

void ConnectionHandlers::remove(ConnectionHandler conn) {
    set<ConnectionHandler>::iterator it = _find_entry(conn);
    if (it == end()) {
        throw "Entry doesn't exist";
    }
    erase(it);
}

ConnectionHandler ConnectionHandlers::get_entry(ConnectionHandler conn) {
    set<ConnectionHandler>::iterator it = _find_entry(conn);
    if (it == end()) {
        throw "Entry doesn't exist";
    }
    return *it;
}

ostream& operator<<(ostream& os, const ConnectionHandlers& ch) {
    set<ConnectionHandler>::iterator it;
    for (it=ch.begin(); it != ch.end(); ++it)
        os << ' ' << *it;
    os << endl;
    return os;
}