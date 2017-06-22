//
// Created by user on 09/06/2017.
//

#ifndef SERVER_TUNNEL_HANDLER_H
#define SERVER_TUNNEL_HANDLER_H

#include "TunnelCommand.h"
#include "TunnelCommandResult.h"

#include "ServerConnection.h"
#include "ConnectionHandlers.h"


class TunnelHandler {
private:
    ServerConnection *conn;
    ConnectionHandlers *conn_list;

    TunnelCommandResult* handle_command(TunnelCommand *cmd);

public:
    TunnelHandler(ServerConnection *conn): conn(conn), conn_list(new ConnectionHandlers()) {};
    TunnelHandler(char *port): TunnelHandler(new ServerConnection(port)) {};
//    ~TunnelHandler();
    void run();
};


#endif //SERVER_TUNNEL_HANDLER_H
