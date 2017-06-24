#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include "TunnelHandler.h"

int __cdecl real_main(int argc, char **argv) {
    // Validate the parameters
    if (argc != 2) {
        cout << "usage: " << argv[0] << " <port>" << endl;
        return 1;
    }
    TunnelHandler tunnel(argv[1]);
    tunnel.run();
    return 0;
}

void test_socket_connection() {
    int buf_len;
    byte_t buf[1024];

    ServerConnection conn("7777");

    conn.wait_for_client();
    buf_len = conn.recv_data((char*)buf, 1024);

//    CommunicateConnectionRequest *cmd = new CommunicateConnectionRequest(ComputerID("localhost", 2), "bla bla");
//    int buf_size = cmd->serialize(buf, MAX_BUFFER_SIZE);
    TunnelRequest *pcmd = parse_request(buf, buf_len);
    cout << pcmd->get_type() << endl;
}


void test_conset() {
    ConnSet b;

    b.add(ComputerID("bla1", 4));
//    b.add(ComputerID("bla2", 5));
//    b.add(ComputerID("bla3", 6));
    try {
//        b.remove(ComputerID("bla2", 15));
    } catch (const char* msg) {
        cout << "Error: " << msg << endl;
    }
    ConnHandler id = b.get_entry(ComputerID("bla1", 4));
    id.communicate("fodoofoo");
//    cout <<  << endl;
}

#define SSS 2048
void test_requests() {
    int buf_len = 0;
    byte_t buf[SSS];

    CommunicateConnectionRequest *cmd = new CommunicateConnectionRequest(ComputerID("localhost", 2), "");
    int buf_size = cmd->serialize(buf, SSS);
    TunnelRequest *pcmd = parse_request(buf, (size_t)buf_size);
    CommunicateConnectionRequest *cmd1 = (CommunicateConnectionRequest*)pcmd;
    cout << cmd1->get_type() << endl;
    cout << cmd1->get_conn_id() << endl;
}

int __cdecl main(int argc, char **argv) {
    return real_main(argc, argv);
//    test_conset();
//    test_requests();
//    return 0;
}