//
// Created by user on 15/06/2017.
//

#include "ConnectionHandler.h"
#include <iostream>

ConnectionHandler::~ConnectionHandler() {
//    cout << "Disconnecting from: " << _host << " on interface: " << _interface << endl;
}

void ConnectionHandler::connect() {
    cout << "Connecting to: " << comm->host << " on interface: " << comm->ifs << endl;
    // TODO: really connect
}

string ConnectionHandler::communicate(string command) {
    cout << comm->host << " sending commnad: " << command << endl;
    // TODO: really communicate
    return command + " sent";
}

bool ConnectionHandler::operator< (const ConnectionHandler& other) const {
    return (comm->host < other.comm->host) || ((comm->host == other.comm->host) && (comm->ifs < other.comm->ifs));
}

bool ConnectionHandler::operator== (const ConnectionHandler &other) {
    return (comm->host == other.comm->host) && (comm->ifs == other.comm->ifs);
}

bool ConnectionHandler::operator!= (const ConnectionHandler &other) {
    return !(*this == other);
}

ostream& operator<<(ostream& os, const ConnectionHandler& ch) {
    os << "(" << ch.comm->host << ", " << ch.comm->ifs << ")";
    return os;
}
