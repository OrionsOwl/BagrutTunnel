//
// Created by user on 15/06/2017.
//

#include "ConnHandler.h"
#include <iostream>

#include <string.h>


bool ConnHandler::operator< (const ConnHandler &other) const { return o < other.o; }
bool ConnHandler::operator== (const ConnHandler &other) { return o == other.o; }
bool ConnHandler::operator!= (const ConnHandler &other) { return o != other.o; }


ConnHandler::~ConnHandler() {
//    cout << "Disconnecting from: " << o << endl;// << " on interface: " << _interface << endl;
}

void ConnHandler::connect() {
    cout << "Connecting to: " << o << endl;//.host << " on interface: " << comm.ifs << endl;
    // TODO: really connect
}

string ConnHandler::communicate(string command) {
    cout << o << " sending commnad: " << command << endl;
    // TODO: really communicate
    return command + " sent";
}


ostream& operator<<(ostream& os, const ConnHandler& ch) {
    os << ch.o;
    return os;
}
