//
// Created by user on 17/06/2017.
//

#ifndef _TUNNEL_REQUEST_H
#define _TUNNEL_REQUEST_H

#include <string>
#include <iostream>
#include <stdint.h>

using namespace std;
typedef unsigned char byte_t;


#define MAX_HOST_NAME (30)

struct ComputerID {
    int ifs;
    char host[MAX_HOST_NAME];
    ComputerID(char *raw_data);
    ComputerID(string host_name, int _ifs);
    bool operator< (const ComputerID& other) const;
    bool operator== (const ComputerID &other);
    bool operator!= (const ComputerID &other) { return !(*this == other); }
    friend ostream& operator<<(ostream& os, const ComputerID& ch);
};


typedef enum command_type_e {
    LIST_INTERFACES,
    OPEN_CONNECTION,
    CLOSE_CONNECTION,
    COMMUNICATE_CONNECTION,
    QUERY_INTERFACE,
} request_type_t;


class TunnelRequest {
private:
    request_type_t cmd_type;
public:
    TunnelRequest(request_type_t cmd_type): cmd_type(cmd_type) {};
    virtual size_t serialize(byte_t *buf, size_t buf_size);
    request_type_t get_type() { return cmd_type; }
    static size_t get_size() { return sizeof(request_type_t); }
};


class ListInterfaceRequest: public TunnelRequest {
public:
    ListInterfaceRequest(): TunnelRequest(LIST_INTERFACES) {};
};


class QueryInterfaceRequest: public TunnelRequest {
private:
    int ifs;
public:
    QueryInterfaceRequest(int ifs) : TunnelRequest(QUERY_INTERFACE), ifs(ifs) {};
    size_t serialize(byte_t *buf, size_t buf_size);
    int get_interface() { return ifs; }
    static size_t get_size() { return TunnelRequest::get_size() + sizeof(int8_t); }
};


class ConnectionRequest: public TunnelRequest {
private:
    ComputerID conn;
public:
    ConnectionRequest(request_type_t type, ComputerID conn): TunnelRequest(type), conn(conn) {}
    ConnectionRequest(request_type_t type, string host, int ifs): ConnectionRequest(type, ComputerID(host, ifs)) {}
    size_t serialize(byte_t *buf, size_t buf_size);
    ComputerID get_conn_id() { return conn; }
    static size_t get_size() { return TunnelRequest::get_size() + sizeof(ComputerID); }
};


class OpenConnectionRequest: public ConnectionRequest {
public:
    OpenConnectionRequest(ComputerID conn): ConnectionRequest(OPEN_CONNECTION, conn) {};
    OpenConnectionRequest(string host, int ifs): OpenConnectionRequest(ComputerID(host, ifs)) {};
    static size_t get_size() { return ConnectionRequest::get_size(); }
};


class CloseConnectionRequest: public ConnectionRequest {
public:
    CloseConnectionRequest(ComputerID conn): ConnectionRequest(CLOSE_CONNECTION, conn) {};
    CloseConnectionRequest(string host, int ifs): CloseConnectionRequest(ComputerID(host, ifs)) {};
    static size_t get_size() { return ConnectionRequest::get_size(); }
};


class CommunicateConnectionRequest: public ConnectionRequest {
private:
    string command;
public:
    CommunicateConnectionRequest(ComputerID conn, string cmd): ConnectionRequest(COMMUNICATE_CONNECTION, conn), command(cmd) {};
    CommunicateConnectionRequest(string host, int ifs, string cmd): CommunicateConnectionRequest(ComputerID(host, ifs), cmd) {};
    size_t serialize(byte_t *buf, size_t buf_size);
    string get_command() { return command; }
    static size_t get_size() { return ConnectionRequest::get_size() + 1; }
};

TunnelRequest* parse_request(byte_t *cmd, size_t command_size);


#endif //_TUNNEL_REQUEST_H
