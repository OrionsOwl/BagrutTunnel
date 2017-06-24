#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <iostream>
//#include "TunnelHandler.h"
#include "ConnSet.h"

int __cdecl main2(int argc, char **argv) {
    // Validate the parameters
    if (argc != 2) {
        cout << "usage: " << argv[0] << " <port>" << endl;
        return 1;
    }
//    TunnelHandler tunnel(argv[1]);
//    tunnel.run();
    return 0;
}
//
//int __cdecl main1(int argc, char **argv) {
//    int buf_len;
//    byte_t buf[MAX_BUFFER_SIZE];
//
//    ServerConnection conn("7777");
//
//    conn.wait_for_client();
//    buf_len = conn.recv_data((char*)buf, MAX_BUFFER_SIZE);
//
////    CommunicateConnectionRequest *cmd = new CommunicateConnectionRequest(ComputerID("localhost", 2), "bla bla");
////    int buf_size = cmd->serialize(buf, MAX_BUFFER_SIZE);
//    TunnelRequest *pcmd = parse_request(buf, buf_len);
//    cout << pcmd->get_type() << endl;
//    return 0;
//}

//int main() {
//    ConnSet b;
//    b.add(ComputerID("bla1", 4));
//    b.add(ComputerID("bla2", 5));
//    b.add(ComputerID("bla3", 6));
////    tst(b);
//    try {
//        b.remove(ComputerID("bla2", 15));
//    } catch (const char* msg) {
//        cout << "Error: " << msg << endl;
//    }
////    ConnHandler a(ComputerID(2, 4));
//    cout << b << endl;
//
//    return 0;
//}

//int __cdecl main2(int argc, char **argv) {
////    TunnelHandler tunnel(argv[1]);
////    ConnSet connections;
////    connections.add(ComputerID("bla", 2));
//    try {
//        tunnel.connections.add(ComputerID("", 4));
//    } catch (exception e) {
//        cout << "Herre" << endl;
//    }
////    b.add(ComputerID("bla2", 5));
////    b.add(ComputerID("bla3", 6));
//    try {
////        b.add(ComputerID("bla2", 5));
//    } catch (const char* msg) {
//        cout << "Error: " << msg << endl;
//    }
////    ConnHandler a(ComputerID(2, 4));
////    cout << b << endl;
////    int buf_len;
////    byte_t buf[MAX_BUFFER_SIZE];
////
////    ServerConnection conn("7777");
////
////    conn.wait_for_client();
////    buf_len = conn.recv_data((char*)buf, MAX_BUFFER_SIZE);
////
////    CommunicateConnectionRequest *cmd = new CommunicateConnectionRequest(ComputerID("localhost", 2), "bla bla");
////    int buf_size = cmd->serialize(buf, MAX_BUFFER_SIZE);
////    TunnelRequest *pcmd = parse_request(buf, buf_len);
////    cout << pcmd->get_type() << endl;
//    return 0;
//}