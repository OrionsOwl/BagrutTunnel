//
// Created by user on 09/06/2017.
//

#include "ConnSet.h"
#include <iostream>

using namespace std;

void ConnSet::add(ConnHandler &conn) {
    if (_find_entry(conn) != end()) {
        throw "Entry already exists";
    }
    conn.connect();
//    insert(conn);
}

void ConnSet::remove(ConnHandler &conn) {
    conn_set_iter it = _find_entry(conn);
    if (it == end()) {
        throw "Entry doesn't exist";
    }
    erase(it);
}

ConnHandler ConnSet::get_entry(ConnHandler &conn) {
    conn_set_iter it = _find_entry(conn);
    if (it == end()) {
        throw "Entry doesn't exist";
    }
    return *it;
}


ostream& operator<<(ostream& os, const ConnSet& ch) {
    conn_set_iter it;
    for (it=ch.begin(); it != ch.end(); ++it)
        os << ' ' << *it;
    os << endl;
    return os;
}
