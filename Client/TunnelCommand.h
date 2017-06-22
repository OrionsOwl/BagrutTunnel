//
// Created by user on 17/06/2017.
//

#ifndef _TUNNEL_COMMAND_H
#define _TUNNEL_COMMAND_H

#include <string>
#include <iostream>
#include <stdint.h>

using namespace std;
typedef unsigned char byte_t;


#define MAX_HOST_NAME (30)

struct ComputerID {
    uint8_t ifs;
    char host[MAX_HOST_NAME];
    ComputerID(char *raw_data);
    ComputerID(string host_name, uint8_t ifs);
    friend ostream& operator<<(ostream& os, const ComputerID& ch);
};


typedef enum command_type_e {
    LIST_INTERFACES,
    OPEN_CONNECTION,
    CLOSE_CONNECTION,
    COMMUNICATE_CONNECTION,
    QUERY_INTERFACE,
    NUM_COMMANDS
} command_type_t;


class TunnelCommand {
private:
    command_type_t cmd_type;
public:
    TunnelCommand(command_type_t cmd_type): cmd_type(cmd_type) {};
    virtual size_t serialize(byte_t *buf, size_t buf_size);
    command_type_t get_type() { return cmd_type; }
    static size_t get_size() { return sizeof(command_type_t); }
};


class ListInterfaceCommand: public TunnelCommand {
public:
    ListInterfaceCommand(): TunnelCommand(LIST_INTERFACES) {};
};


class QueryInterfaceCommand: public TunnelCommand {
private:
    uint8_t ifs;
public:
    QueryInterfaceCommand(uint8_t ifs) : TunnelCommand(QUERY_INTERFACE), ifs(ifs) {};
    size_t serialize(byte_t *buf, size_t buf_size);
    uint8_t get_interface() { return ifs; }
    static size_t get_size() { return TunnelCommand::get_size() + sizeof(int8_t); }
};


class ConnectionCommand: public TunnelCommand {
private:
    ComputerID conn;
public:
    ConnectionCommand(command_type_t type, ComputerID conn): TunnelCommand(type), conn(conn) {}
    ConnectionCommand(command_type_t type, string host, uint8_t ifs): ConnectionCommand(type, ComputerID(host, ifs)) {}
    size_t serialize(byte_t *buf, size_t buf_size);
    ComputerID* get_conn_id() { return &conn; }
    static size_t get_size() { return TunnelCommand::get_size() + sizeof(ComputerID); }
};


class OpenConnectionCommand: public ConnectionCommand {
public:
    OpenConnectionCommand(ComputerID conn): ConnectionCommand(OPEN_CONNECTION, conn) {};
    OpenConnectionCommand(string host, uint8_t ifs): OpenConnectionCommand(ComputerID(host, ifs)) {};
    static size_t get_size() { return ConnectionCommand::get_size(); }
};


class CloseConnectionCommand: public ConnectionCommand {
public:
    CloseConnectionCommand(ComputerID conn): ConnectionCommand(CLOSE_CONNECTION, conn) {};
    CloseConnectionCommand(string host, uint8_t ifs): CloseConnectionCommand(ComputerID(host, ifs)) {};
    static size_t get_size() { return ConnectionCommand::get_size(); }
};


class CommunicateConnectionCommand: public ConnectionCommand {
private:
    string command;
public:
    CommunicateConnectionCommand(ComputerID conn, string cmd): ConnectionCommand(COMMUNICATE_CONNECTION, conn), command(cmd) {};
    CommunicateConnectionCommand(string host, uint8_t ifs, string cmd): CommunicateConnectionCommand(ComputerID(host, ifs), cmd) {};
    size_t serialize(byte_t *buf, size_t buf_size);
    string get_command() { return command; }
    static size_t get_size() { return ConnectionCommand::get_size() + 1; }
};

TunnelCommand* parse_command(byte_t *cmd, size_t command_size);


#endif //_TUNNEL_COMMAND_H
