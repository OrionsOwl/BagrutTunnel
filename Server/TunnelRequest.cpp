//
// Created by user on 17/06/2017.
//

#include <string.h>
#include "TunnelRequest.h"


ComputerID::ComputerID(string host_name, uint8_t _ifs) {
    if (host_name.length() >= MAX_HOST_NAME) {
        throw std::length_error("Given host name is too long");
    }
    memset(host, 0, MAX_HOST_NAME);
    memmove(host, host_name.c_str(), host_name.length());
    host[host_name.length()] = 0;
    ifs = _ifs;
}

ComputerID::ComputerID(char *raw_data) {
    ifs = (uint8_t)raw_data[0];
    memmove(host, raw_data + sizeof(uint8_t), MAX_HOST_NAME);
}

bool ComputerID::operator< (const ComputerID& other) const {
    if (ifs != other.ifs)
        return ifs < other.ifs;
    return bool(strcmp(host, other.host));
}

bool ComputerID::operator== (const ComputerID &other) {
    return ifs == other.ifs && string(host) == string(other.host);
}

ostream& operator<<(ostream& os, const ComputerID& ch) {
    os << "(" << ch.host << ", " << int(ch.ifs) << ")";
    return os;
}

// ############################################


size_t TunnelRequest::serialize(byte_t *buf, size_t buf_size) {
    size_t needed_size = sizeof(cmd_type);
    if (needed_size > buf_size) {
        throw length_error("Buff size is not sufficient");
    }
    memmove(buf, &cmd_type, needed_size);
    return needed_size;
}


size_t ConnectionRequest::serialize(byte_t *buf, size_t buf_size) {
    size_t needed_size = sizeof(conn);
    if (needed_size > buf_size) {
        throw length_error("Buff size is not sufficient");
    }
    size_t res = TunnelRequest::serialize(buf, buf_size - needed_size);
    memmove(buf + res, &conn, sizeof(conn));
    return res + needed_size;
}


size_t CommunicateConnectionRequest::serialize(byte_t *buf, size_t buf_size) {
    size_t needed_size = command.length();
    if (needed_size > buf_size) {
        throw length_error("Buff size is not sufficient");
    }
    size_t res = ConnectionRequest::serialize(buf, buf_size - needed_size);
    memmove(buf + res, command.c_str(), needed_size);
    return res + needed_size;
}


size_t QueryInterfaceRequest::serialize(byte_t *buf, size_t buf_size) {
    size_t needed_size = sizeof(ifs);
    if (needed_size > buf_size) {
        throw length_error("Buff size is not sufficient");
    }
    size_t res = TunnelRequest::serialize(buf, buf_size - needed_size);
    memmove(buf + res, &ifs, needed_size);
    return res + needed_size;
}


TunnelRequest* parse_request(byte_t *cmd, size_t command_size) {
    if (TunnelRequest::get_size() > command_size) {
        throw length_error("Command has invalid length");
    }
    request_type_t cmd_type;
    memmove(&cmd_type, cmd, sizeof(request_type_t));
//    request_type_t cmd_type = (request_type_t)*cmd;
    cmd += sizeof(request_type_t);
    cout << "Received command: " << cmd_type << endl;
    switch (cmd_type) {
        case LIST_INTERFACES:
            if (ListInterfaceRequest::get_size() != command_size) {
                throw length_error("Command has invalid length");
            }
            return new ListInterfaceRequest();
        case QUERY_INTERFACE:
            if (QueryInterfaceRequest::get_size() != command_size) {
                throw length_error("Command has invalid length");
            }
            return new QueryInterfaceRequest((uint8_t)cmd[0]);
        case OPEN_CONNECTION:
            if (OpenConnectionRequest::get_size() != command_size) {
                throw length_error("Command has invalid length");
            }
            return new OpenConnectionRequest(ComputerID((char*)cmd));
        case CLOSE_CONNECTION:
            if (CloseConnectionRequest::get_size() != command_size) {
                throw length_error("Command has invalid length");
            }
            return new CloseConnectionRequest(ComputerID((char*)cmd));
        case COMMUNICATE_CONNECTION:
            if (CommunicateConnectionRequest::get_size() > command_size) {
                throw length_error("Command has invalid length");
            }
            cmd[command_size - sizeof(request_type_t)] = '\0';
            return new CommunicateConnectionRequest(ComputerID((char*)cmd), string((char*)cmd + sizeof(ComputerID)));
        default:
            throw invalid_argument("Invalid command type");
    }
}