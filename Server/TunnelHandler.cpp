//
// Created by user on 09/06/2017.
//

#include "TunnelHandler.h"

using namespace std;

#define MAX_BUF_LEN (4096 * 5)


void TunnelHandler::run() {
    int buf_len;
    TunnelRequest *cmd = NULL;
    TunnelResponse *res = NULL;
    char buf[MAX_BUF_LEN];

    conn->wait_for_client();

    while (true) {
        buf_len = conn->recv_data(buf, MAX_BUF_LEN);
        if (0 > buf_len) {
            cout << "Connection closed" << endl;
            conn->close_client();
            conn->wait_for_client();
            continue;
        }

        res = NULL;
        try {
            cmd = parse_request((byte_t *) buf, (size_t) buf_len);
        } catch (const char *msg) {
            res = new ErrorResponse(cmd->get_type(), string(msg));
        }
        if (NULL == res) {
            try {
                res = handle_command(cmd);
            } catch (invalid_argument err) {
                res = new ErrorResponse(cmd->get_type(), err.what());
            } catch (length_error err) {
                res = new ErrorResponse(cmd->get_type(), err.what());
            } catch (const char *msg) {
                res = new ErrorResponse(cmd->get_type(), string(msg));
            }
        }
        buf_len = res->serialize((byte_t*)buf, MAX_BUF_LEN);
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
                return new ErrorResponse(cmd->get_type(), msg);
            }
        }
        case QUERY_INTERFACE: {
            string query_data = "";
            QueryInterfaceRequest *query_cmd = (QueryInterfaceRequest*)cmd;
            int ifs = query_cmd->get_interface();
            for(conn_set_iter iter=connections.begin(); iter != connections.end(); iter++) {
                const ComputerID id = iter->get_id();
                if (id.ifs == ifs) {
                    query_data += ", " + string(id.host);
                }
            }
            return new DataResponse(cmd->get_type(), query_data);
        }
        default:
            throw invalid_argument("Invalid command type");
    }
    return new AckResponse(cmd->get_type());
}

