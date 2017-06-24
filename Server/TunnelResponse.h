//
// Created by user on 21/06/2017.
//

#ifndef _TUNNEL_RESPONSE_H
#define _TUNNEL_RESPONSE_H

#include "TunnelRequest.h"

typedef enum response_type_e {
    ACK_RESPONSE,
    ERROR_RESPONSE,
    DATA_RESPONSE,
} response_type_t;


class TunnelResponse {
private:
    request_type_t cmd_type;
    response_type_t res_type;
public:
    TunnelResponse(response_type_t res_type, request_type_t cmd_type): cmd_type(cmd_type), res_type(res_type) {};
    virtual size_t serialize(byte_t *buf, size_t buf_size);
    request_type_t get_type() { return cmd_type; }
    response_type_t get_res_type() { return res_type; }
    static size_t get_size() { return sizeof(response_type_t) + sizeof(request_type_t); }
};

class AckResponse: public TunnelResponse {
public:
    AckResponse(request_type_t cmd_type): TunnelResponse(ACK_RESPONSE, cmd_type) {};
};


class StringResponse: public TunnelResponse {
private:
    string info;
public:
    StringResponse(response_type_t res_type, request_type_t cmd_type): TunnelResponse(res_type, cmd_type) {};
    StringResponse(response_type_t res_type, request_type_t cmd_type, string info):
            TunnelResponse(res_type, cmd_type), info(info) {};
    size_t serialize(byte_t *buf, size_t buf_size);
    string get_data() { return info; }
    void set_data(string data_info) { info = data_info; }
    static size_t get_size() { return TunnelResponse::get_size(); }
};

class ErrorResponse: public StringResponse {
public:
    ErrorResponse(request_type_t cmd_type): StringResponse(ERROR_RESPONSE, cmd_type) {};
    ErrorResponse(request_type_t cmd_type, string info): StringResponse(ERROR_RESPONSE, cmd_type, info) {};
    static size_t get_size() { return StringResponse::get_size(); }
};

class DataResponse: public StringResponse {
public:
    DataResponse(request_type_t cmd_type): StringResponse(DATA_RESPONSE, cmd_type) {};
    DataResponse(request_type_t cmd_type, string info): StringResponse(DATA_RESPONSE, cmd_type, info) {};
    static size_t get_size() { return StringResponse::get_size(); }
};

TunnelResponse* parse_response(byte_t *cmd, size_t command_size);


#endif //_TUNNEL_RESPONSE_H
