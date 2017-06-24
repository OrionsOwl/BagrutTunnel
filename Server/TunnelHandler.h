//
// Created by user on 09/06/2017.
//

#ifndef SERVER_TUNNEL_HANDLER_H
#define SERVER_TUNNEL_HANDLER_H

#include "TunnelRequest.h"
#include "TunnelResponse.h"

#include "ServerConnection.h"
#include "ConnSet.h"


class TunnelHandler {
public://private:
    ServerConnection *conn;
    ConnSet connections;

    /*
     * @brief: Executes a specific command
     * @param cmd: The command to execute
     * @return command_result: The result of the execution (Ack, Error, Data)
     * @note:
     */
    TunnelResponse* handle_command(TunnelRequest *cmd);

public:

    /*
     * Open a connection
     */
    TunnelHandler(ServerConnection *conn): conn(conn) {};
    TunnelHandler(char *port): TunnelHandler(new ServerConnection(port)) {};
//    ~TunnelHandler();
    /*
     * Executes the main loop of the Tunnel
     */
    void run();
};


#endif //SERVER_TUNNEL_HANDLER_H
