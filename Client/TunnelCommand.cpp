//
// Created by user on 17/06/2017.
//

#include <string.h>
#include "TunnelCommand.h"

ostream& operator<<(ostream& os, const ComputerID& ch) {
    os << ch.host;
    return os;
}

ComputerID::ComputerID(string host_name, uint8_t ifs): ifs(ifs) {
    if (host_name.length() >= MAX_HOST_NAME) {
        throw std::length_error("Given host name is too long");
    }
    memset(host, 0, MAX_HOST_NAME);
    memmove(host, host_name.c_str(), host_name.length());
    host[host_name.length()] = 0;
}

ComputerID::ComputerID(char *raw_data) {
    ifs = (uint8_t)raw_data[0];
    memmove(host, raw_data + 1, MAX_HOST_NAME);
}


size_t TunnelCommand::serialize(byte_t *buf, size_t buf_size) {
    size_t needed_size = sizeof(cmd_type);
    if (needed_size > buf_size) {
        throw length_error("Buff size is not sufficient");
    }
    memmove(buf, &cmd_type, needed_size);
    return needed_size;
}


size_t ConnectionCommand::serialize(byte_t *buf, size_t buf_size) {
    size_t needed_size = sizeof(conn);
    if (needed_size > buf_size) {
        throw length_error("Buff size is not sufficient");
    }
    size_t res = TunnelCommand::serialize(buf, buf_size - needed_size);
    memmove(buf + res, &conn, sizeof(conn));
    return res + needed_size;
}


size_t CommunicateConnectionCommand::serialize(byte_t *buf, size_t buf_size) {
    size_t needed_size = command.length();
    if (needed_size > buf_size) {
        throw length_error("Buff size is not sufficient");
    }
    size_t res = ConnectionCommand::serialize(buf, buf_size - needed_size);
    memmove(buf + res, command.c_str(), needed_size);
    return res + needed_size;
}


size_t QueryInterfaceCommand::serialize(byte_t *buf, size_t buf_size) {
    size_t needed_size = sizeof(ifs);
    if (needed_size > buf_size) {
        throw length_error("Buff size is not sufficient");
    }
    size_t res = TunnelCommand::serialize(buf, buf_size - needed_size);
    memmove(buf + res, &ifs, needed_size);
    return res + needed_size;
}


TunnelCommand* parse_command(byte_t *cmd, size_t command_size) {
    if (TunnelCommand::get_size() > command_size) {
        throw length_error("Command has invalid length");
    }
    command_type_t cmd_type = (command_type_t)cmd[0];
    cmd += sizeof(command_type_t);

    switch (cmd_type) {
        case LIST_INTERFACES:
            if (ListInterfaceCommand::get_size() != command_size) {
                throw length_error("Command has invalid length");
            }
            return new ListInterfaceCommand();
        case QUERY_INTERFACE:
            if (QueryInterfaceCommand::get_size() != command_size) {
                throw length_error("Command has invalid length");
            }
            return new QueryInterfaceCommand((uint8_t)cmd[0]);
        case OPEN_CONNECTION:
            if (OpenConnectionCommand::get_size() != command_size) {
                throw length_error("Command has invalid length");
            }
            return new OpenConnectionCommand(ComputerID((char*)cmd));
        case CLOSE_CONNECTION:
            if (CloseConnectionCommand::get_size() != command_size) {
                throw length_error("Command has invalid length");
            }
            return new CloseConnectionCommand(ComputerID((char*)cmd));
        case COMMUNICATE_CONNECTION:
            if (CommunicateConnectionCommand::get_size() > command_size) {
                throw length_error("Command has invalid length");
            }
            cmd[command_size - sizeof(command_type_t)] = '\0';
            return new CommunicateConnectionCommand(ComputerID((char*)cmd), string((char*)cmd + sizeof(ComputerID)));
        default:
            throw invalid_argument("Invalid command type");
    }
}