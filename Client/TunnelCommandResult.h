//
// Created by user on 21/06/2017.
//

#ifndef _TUNNEL_COMMAND_RESULT_H
#define _TUNNEL_COMMAND_RESULT_H

#include "TunnelCommand.h"

#pragma pack(1)
typedef enum command_result_type_e {
    ACK_RESULT,
    ERROR_RESULT,
    DATA_RESULT,
} command_result_type_t;
#pragma unpack


class TunnelCommandResult {
private:
    command_type_t cmd_type;
    command_result_type_t res_type;
public:
    TunnelCommandResult(command_result_type_t res_type, command_type_t cmd_type): cmd_type(cmd_type), res_type(res_type) {};
    virtual size_t serialize(byte_t *buf, size_t buf_size);
    command_type_t get_type() { return cmd_type; }
    command_result_type_t get_res_type() { return res_type; }
    static size_t get_size() { return sizeof(command_result_type_t) + sizeof(command_type_t); }
};

class AckCommandResult: public TunnelCommandResult {
public:
    AckCommandResult(command_type_t cmd_type): TunnelCommandResult(ACK_RESULT, cmd_type) {};
};


class StringCommandResult: public TunnelCommandResult {
private:
    string info;
public:
    StringCommandResult(command_result_type_t res_type, command_type_t cmd_type): TunnelCommandResult(res_type, cmd_type) {};
    StringCommandResult(command_result_type_t res_type, command_type_t cmd_type, string info):
            TunnelCommandResult(res_type, cmd_type), info(info) {};
    size_t serialize(byte_t *buf, size_t buf_size);
    string get_data() { return info; }
    void set_data(string data_info) { info = data_info; }
    static size_t get_size() { return TunnelCommandResult::get_size() + 1; }
};

class ErrorCommandResult: public StringCommandResult {
public:
    ErrorCommandResult(command_type_t cmd_type): StringCommandResult(ERROR_RESULT, cmd_type) {};
    ErrorCommandResult(command_type_t cmd_type, string info): StringCommandResult(ERROR_RESULT, cmd_type, info) {};
    static size_t get_size() { return StringCommandResult::get_size(); }
};

class DataCommandResult: public StringCommandResult {
public:
    DataCommandResult(command_type_t cmd_type): StringCommandResult(DATA_RESULT, cmd_type) {};
    DataCommandResult(command_type_t cmd_type, string info): StringCommandResult(DATA_RESULT, cmd_type, info) {};
    static size_t get_size() { return StringCommandResult::get_size(); }
};

TunnelCommandResult* parse_command_result(byte_t *cmd, size_t command_size);


#endif //_TUNNEL_COMMAND_RESULT_H
