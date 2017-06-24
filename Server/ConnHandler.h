/*
 * Created by user on 15/06/2017.
 * @brief:
*/

#ifndef SERVER_CONNECTION_HANDLER_H
#define SERVER_CONNECTION_HANDLER_H

#include <string>
#include "TunnelRequest.h"

using namespace std;

class ConnHandler {
private:
    ComputerID o;

public:
    ConnHandler(ComputerID &_o, bool should_connect): o(_o) {};
    ConnHandler(ComputerID &_o): ConnHandler(_o, false) {};
    ~ConnHandler();
    void connect();
    string communicate(string command);

    bool operator< (const ConnHandler& other) const;
    bool operator== (const ConnHandler &other);
    bool operator!= (const ConnHandler &other);
    ComputerID get_id() const { return o; };
    friend ostream& operator<<(ostream& os, const ConnHandler& ch);
};


#endif //SERVER_CONNECTION_HANDLER_H
