//
// Created by user on 21/06/2017.
//

#include "TunnelCommandResult.h"

size_t TunnelCommandResult::serialize(byte_t *buf, size_t buf_size) {
    size_t needed_size = sizeof(res_type) + sizeof(cmd_type);
    if (needed_size > buf_size) {
        throw length_error("Buff size is not sufficient");
    }
    memmove(buf, &res_type, sizeof(command_result_type_t));
    memmove(buf + sizeof(command_result_type_t), &cmd_type, sizeof(command_type_t));
    return needed_size;
}

size_t StringCommandResult::serialize(byte_t *buf, size_t buf_size) {
    size_t needed_size = info.length();
    if (needed_size > buf_size) {
        throw length_error("Buff size is not sufficient");
    }
    size_t res = TunnelCommandResult::serialize(buf, buf_size - needed_size);
    memmove(buf + res, info.c_str(), needed_size);
    return res + needed_size;
}

TunnelCommandResult* parse_command_result(byte_t *cmd, size_t result_size) {
    if (TunnelCommandResult::get_size() > result_size) {
        throw length_error("Command result has invalid length");
    }
    command_result_type_t res_type = (command_result_type_t)cmd[0];
    cmd += sizeof(command_result_type_t);
    command_type_t cmd_type = (command_type_t)cmd[0];
    cmd += sizeof(command_type_t);

    switch (res_type) {
        case ACK_RESULT:
            if (AckCommandResult::get_size() != result_size) {
                throw length_error("Command result has invalid length");
            }
            return new AckCommandResult(cmd_type);
        case DATA_RESULT:
            if (DataCommandResult::get_size() > result_size) {
                throw length_error("Command result has invalid length");
            }
            cmd[result_size - sizeof(command_type_t) - sizeof(command_result_type_t)] = '\0';
            return new DataCommandResult(cmd_type, string((char*)cmd));
        case ERROR_RESULT:
            if (ErrorCommandResult::get_size() > result_size) {
                throw length_error("Command result has invalid length");
            }
            cmd[result_size - sizeof(command_type_t) - sizeof(command_result_type_t)] = '\0';
            return new ErrorCommandResult(cmd_type, string((char*)cmd));
        default:
            throw invalid_argument("Invalid result type");
    }
}