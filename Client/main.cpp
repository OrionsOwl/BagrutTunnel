#define WIN32_LEAN_AND_MEAN

#include <stdlib.h>
#include <string>
#include <iostream>

#include "ClientConnection.h"
#include "TunnelCommand.h"

using namespace std;

#define MAX_BUFFER_SIZE (1024)
#define DEFAULT_PORT "27015"
#define DEFAULT_HOST "localhost"


bool verify_exit_choice() {
    cout << "Are you sure you want to exit?" << endl;
    cout << "1. Yes" << endl;
    cout << "2. No" << endl;
    int choice = 0;
    cin >> choice;
    return choice == 1;
}

//string interfaces[] = {"eth0", "eth1",};

uint8_t get_interface_from_user() {
    uint8_t ifs;
    cout << "Please choose interface " << endl;
    cout << "1. eth0" << endl;
    cout << "2. eth1" << endl;
//    for (int i = 0; i < interfaces->length(); i++) {
//        cout << i + 1 << ". " << interfaces[i] << endl;
//    }
    cin >> ifs;
//    if (ifs > interfaces->length()) {
//        cout << "Invalid interface choice" << endl;
//        throw ;
//    }
    return ifs;
}

string get_host_from_user() {
    string host;

    cout << "Enter the IP address of the destination computer: ";
    getline(cin, host);
    if (host.length() > MAX_HOST_NAME) {
        cout << "Invalid host - should be no longer than " << MAX_HOST_NAME << " characters" << endl;
//        throw ;
    }
    return host;
}

ComputerID* query_connection(){
    string host = get_host_from_user();
    uint8_t ifs = get_interface_from_user();
    return new ComputerID(host, ifs);
}

TunnelCommand* list_cmd_handler() { //TODO: check where to initialize the interface
    return new ListInterfaceCommand();
}


TunnelCommand* communicate_cmd_handler() {
    ComputerID *id = query_connection();
    cout << "Enter command: " << endl;
    string command;
    cin >> command;
    return new CommunicateConnectionCommand(*id, command);
}

TunnelCommand* query_cmd_handler() {
    uint8_t ifs = get_interface_from_user();
    return new QueryInterfaceCommand(ifs);
}

TunnelCommand* connect_cmd_handler() {
    ComputerID *id = query_connection();
    return new OpenConnectionCommand(*id);
}

TunnelCommand* disconnect_cmd_handler() {
    ComputerID *id = query_connection();
    return new CloseConnectionCommand(*id);
}


//class MenuUser {
//private:
//    ComputerID *id;
//public:
//    MenuUser();
//    void bla() {
//        while (true) {
//
//        }
//    }
//};


#define MENU_TABLE \
    MENU_ITEM(IFS_LIST, "interface list", list_cmd_handler) \
    MENU_ITEM(CONNECT_IFS, "connect to machine", connect_cmd_handler) \
    MENU_ITEM(DISCONNECT_IFS, "disconnect from machine", disconnect_cmd_handler) \
    MENU_ITEM(COMMUNICATE_IFS, "talk to machine", communicate_cmd_handler) \
    MENU_ITEM(QUERY_IFS, "query interface", query_cmd_handler) \
    MENU_ITEM(EXIT, "exit", NULL) \
    MENU_ITEM(NUM_ITEMS, "", NULL)

#define MENU_ITEM(item_num, item_str, handler) item_num,
typedef enum option_e {
    MENU_TABLE
} option_t;
#undef MENU_ITEM

#define MENU_ITEM(item_num, item_str, handler) item_str,
char *options_str[] = {
        MENU_TABLE
};
#undef MENU_ITEM

typedef TunnelCommand* (*cmd_handler)(void);

#define MENU_ITEM(item_num, item_str, handler) handler,
cmd_handler cmd_handlers[] = {
        MENU_TABLE
};
#undef MENU_ITEM

TunnelCommand* tunnel_menu() {
    int choice = 0;

    while (true) {
        cout << "What do you want to do:" << endl;
        for (int i = 0; i < NUM_ITEMS; i++) {
            cout << i + 1 << ". " << options_str[i] << endl;
        }
        cin >> choice;
        choice -= 1;
        if (EXIT == choice) {
            if (verify_exit_choice()) {
                return NULL;
            }
//            choice = 0;
            continue;
        }
        if (choice >= NUM_ITEMS || choice < 0) {
            cout << "Bad choice: should enter a valid option" << endl;
            continue;
        }
        return cmd_handlers[choice]();
    }
}


//void send_and_receive_command(TunnelCommand *cmd, ) {
//    //        CommunicateConnectionCommand *upcmd = (CommunicateConnectionCommand*)cmd;
//    int buf_size = cmd->serialize((byte_t*)buf, MAX_BUFFER_SIZE);
//    int written_bytes = conn.send_buffer(buf, (size_t)buf_size);
//    cout << "Written bytes: " << written_bytes << endl;
//    conn.recv_data(buf, (size_t)MAX_BUFFER_SIZE);
//    cout << buf << endl;
//}


void run(string server_name, string port) {
    char buf[MAX_BUFFER_SIZE];
    bool should_continue;

    ClientConnection conn(server_name, port);

    while (true) {
        TunnelCommand *cmd = tunnel_menu();
        if (NULL == cmd) {
            cout << "Bye bye" << endl;
            break;
        }
        if (OPEN_CONNECTION == cmd->get_type()) {
            TunnelCommand *cur_cmd = NULL;
            OpenConnectionCommand *open_cmd = (OpenConnectionCommand*)cmd;
            ComputerID *current_id = open_cmd->get_conn_id();
            string command;
            should_continue = true;

            conn.send_and_receive(open_cmd);
            while (should_continue) {
                cout << *current_id << ": ";
                getline(cin, command);
                if ("exit" == command) {
                    cur_cmd = new CloseConnectionCommand(*current_id);
                    should_continue = false;
                } else {
                    cur_cmd = new CommunicateConnectionCommand(*current_id, command);
                }
                conn.send_and_receive(cur_cmd);
            }
        }
        conn.send_and_receive(cmd);
    }
}

int __cdecl main(int argc, char **argv) {
    // Validate the parameters
    if (argc != 3) {
        printf("usage: %s <server_name> <port>\n", argv[0]);
        return 1;
    }
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));

//    string a;
//    cout << "Press me:  ";
//    getline(cin, a);
//    cout << "Entered: " << a;
    run(argv[1], argv[2]);
//    test_client_connection();
//    ComputerID *current_id = new ComputerID("blabla", 123);
//    cout << *current_id << ": ";
    return 0;
}