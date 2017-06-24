//
// Created by user on 09/06/2017.
//

#include "TunnelHandler.h"
#include "SocketConnection.h"

using namespace std;

#define DEFAULT_BUFLEN (4096)


void TunnelHandler::run() {
    int buf_len;
    char buf[DEFAULT_BUFLEN];

    conn->wait_for_client();

    while (true) {
        buf_len = conn->recv_data(buf, DEFAULT_BUFLEN);
        if (0 > buf_len) {
            cout << "Connection closed" << endl;
            conn->close_client();
            break;
//            conn->wait_for_client();
//            continue;
        }

        TunnelRequest *cmd = parse_request((byte_t *) buf, (size_t) buf_len);

        TunnelResponse *res = handle_command(cmd);
        buf_len = res->serialize((byte_t*)buf, DEFAULT_BUFLEN);
        buf_len = conn->send_buffer(buf, (size_t)buf_len);
        if (0 > buf_len) {
            conn->close_client();
            break;
        }
    }
}


TunnelResponse* TunnelHandler::handle_command(TunnelRequest *cmd) {
    switch (cmd->get_type()) {
//        case LIST_INTERFACES:
//            break;
        case OPEN_CONNECTION: {
            OpenConnectionRequest *open_cmd = (OpenConnectionRequest *) cmd;
            try {
                connections.add(open_cmd->get_conn_id());
            } catch (const char *msg) {
//                res.result_str = "Error while opening connection";
//                res.result_str = "Session already open";
                return new ErrorResponse(cmd->get_type(), string(msg));
            }
            break;
        }
        case CLOSE_CONNECTION: {
            CloseConnectionRequest *close_cmd = (CloseConnectionRequest *) cmd;
            try {
                connections.remove(close_cmd->get_conn_id());
            } catch (const char *msg) {
                return new ErrorResponse(cmd->get_type(), "Error while closing connection");
            }
            break;
        }
        case COMMUNICATE_CONNECTION: {
            CommunicateConnectionRequest *comm_cmd = (CommunicateConnectionRequest *) cmd;
            try {
                ConnHandler conn = connections.get_entry(comm_cmd->get_conn_id());
                return new DataResponse(cmd->get_type(), conn.communicate(comm_cmd->get_command()));
            } catch (const char *msg) {
                return new ErrorResponse(cmd->get_type(), "Session wasn't found");
//                return new ErrorResponse(cmd->get_type(), "Error communicating with session");
            }
        }
        case QUERY_INTERFACE: {
            string query_data = "";
            QueryInterfaceRequest *query_cmd = (QueryInterfaceRequest*)cmd;
//            uint8_t ifs = query_cmd->get_interface();
//            for(connection_handlers_iter iter=connections->begin(); iter != connections->end(); iter++) {
//                ComputerID *id = iter->get_id();
//                if (id->ifs == ifs) {
//                    query_data += ", " + string(id->host);
//                }
//            }
            return new DataResponse(cmd->get_type(), query_data);
        }
        default:
            throw invalid_argument("Invalid command type");
    }
    return new AckResponse(cmd->get_type());
}

