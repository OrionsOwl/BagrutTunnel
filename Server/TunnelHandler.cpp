//
// Created by user on 09/06/2017.
//

#include "TunnelHandler.h"
#include "SocketConnection.h"

using namespace std;

#define DEFAULT_BUFLEN (1024)

//TunnelHandler::TunnelHandler(ServerConnection conn) {
//}

//int res = 0;
//int count = 0;
//int buflen = 0;
//char recvbuf[DEFAULT_BUFLEN];
//Server conn(DEFAULT_PORT);
//
//for (count = 0; count < 5; count++) {
//conn.wait_for_client();
//res = conn.recv_data(recvbuf, DEFAULT_BUFLEN, &buflen);
//if (0 > res) {
//conn.close_client();
//continue;
//}
//res = conn.send_buffer(recvbuf, buflen);
//if (0 > res) {
//conn.close_client();
//}
//}

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

        TunnelCommand *cmd = parse_command((byte_t *)buf, (size_t)buf_len);

        TunnelCommandResult *res = handle_command(cmd);
        buf_len = res->serialize((byte_t*)buf, DEFAULT_BUFLEN);
        buf_len = conn->send_buffer(buf, buf_len);
        if (0 > buf_len) {
            conn->close_client();
            break;
        }
    }
}

TunnelCommandResult* TunnelHandler::handle_command(TunnelCommand *cmd) {
    switch (cmd->get_type()) {
//        case LIST_INTERFACES:
//            break;
        case OPEN_CONNECTION: {
            OpenConnectionCommand *open_cmd = (OpenConnectionCommand *) cmd;
            try {
                conn_list->add(open_cmd->get_conn_id());
            } catch (const char *msg) {
//                res.result_str = "Error while opening connection";
//                res.result_str = "Session already open";
                return new ErrorCommandResult(cmd->get_type(), string(msg));
            }
            break;
        }
        case CLOSE_CONNECTION: {
            CloseConnectionCommand *close_cmd = (CloseConnectionCommand *) cmd;
            try {
                conn_list->remove(close_cmd->get_conn_id());
            } catch (const char *msg) {
                return new ErrorCommandResult(cmd->get_type(), "Error while closing connection");
            }
            break;
        }
        case COMMUNICATE_CONNECTION: {
            CommunicateConnectionCommand *comm_cmd = (CommunicateConnectionCommand *) cmd;
            try {
                ConnectionHandler conn = conn_list->get_entry(comm_cmd->get_conn_id());
                return new DataCommandResult(cmd->get_type(), conn.communicate(comm_cmd->get_command()));
            } catch (const char *msg) {
                return new ErrorCommandResult(cmd->get_type(), "Session wasn't found");
//                return new ErrorCommandResult(cmd->get_type(), "Error communicating with session");
            }
        }
        case QUERY_INTERFACE: {
            string query_data = "";
//            QueryInterfaceCommand *query_cmd = (QueryInterfaceCommand*)cmd;
//            uint8_t ifs = query_cmd->get_interface();
//            for(connection_handlers_iter iter=conn_list->begin(); iter != conn_list->end(); iter++) {
//                ComputerID *id = iter->get_id();
//                if (id->ifs == ifs) {
//                    query_data += ", " + string(id->host);
//                }
//            }
            return new DataCommandResult(cmd->get_type(), query_data);
        }
        default:
            throw invalid_argument("Invalid command type");
    }
    return new AckCommandResult(cmd->get_type());
}

