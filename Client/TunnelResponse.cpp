//
// Created by user on 21/06/2017.
//

#include "TunnelResponse.h"

size_t TunnelResponse::serialize(byte_t *buf, size_t buf_size) {
    size_t needed_size = sizeof(res_type) + sizeof(cmd_type);
    if (needed_size > buf_size) {
        throw length_error("Buff size is not sufficient");
    }
    memmove(buf, &res_type, sizeof(response_type_t));
    memmove(buf + sizeof(response_type_t), &cmd_type, sizeof(request_type_t));
    return needed_size;
}


size_t StringResponse::serialize(byte_t *buf, size_t buf_size) {
    size_t needed_size = info.length();
    if (needed_size > buf_size) {
        throw length_error("Buff size is not sufficient");
    }
    size_t res = TunnelResponse::serialize(buf, buf_size - needed_size);
    memmove(buf + res, info.c_str(), needed_size);
    return res + needed_size;
}


TunnelResponse* parse_response(byte_t *cmd, size_t result_size) {
    if (TunnelResponse::get_size() > result_size) {
        throw length_error("Command result has invalid length");
    }
    response_type_t res_type;// = (response_type_t)cmd[0];
    memmove(&res_type, cmd, sizeof(response_type_t));
    cmd += sizeof(response_type_t);
    request_type_t cmd_type;// = (request_type_t)cmd[0];
    memmove(&cmd_type, cmd, sizeof(request_type_t));
    cmd += sizeof(request_type_t);

    switch (res_type) {
        case ACK_RESPONSE:
            if (AckResponse::get_size() != result_size) {
                throw length_error("Command result has invalid length");
            }
            return new AckResponse(cmd_type);
        case DATA_RESPONSE:
            if (DataResponse::get_size() > result_size) {
                throw length_error("Command result has invalid length");
            }
            cmd[result_size - sizeof(request_type_t) - sizeof(response_type_t)] = '\0';
            return new DataResponse(cmd_type, string((char*)cmd));
        case ERROR_RESPONSE:
            if (ErrorResponse::get_size() > result_size) {
                throw length_error("Command result has invalid length");
            }
            cmd[result_size - sizeof(request_type_t) - sizeof(response_type_t)] = '\0';
            return new ErrorResponse(cmd_type, string((char*)cmd));
        default:
            throw invalid_argument("Invalid result type");
    }
}