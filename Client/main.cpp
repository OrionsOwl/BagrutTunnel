#define WIN32_LEAN_AND_MEAN

#include <stdlib.h>
#include <string>
#include <iostream>

#include "ClientConnection.h"
#include "TunnelRequest.h"
#include <limits>

using namespace std;

#define MAX_BUFFER_SIZE (1024)


bool verify_exit_choice() {
    cout << "Are you sure you want to exit?" << endl;
    cout << "1. Yes" << endl;
    cout << "2. No" << endl;
    int choice = 0;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice == 1;
}

//string interfaces[] = {"eth0", "eth1",};

int get_interface_from_user() {
    int ifs;
    cout << "Please choose interface " << endl;
    cout << "1. eth0" << endl;
    cout << "2. eth1" << endl;
//    for (int i = 0; i < interfaces->length(); i++) {
//        cout << i + 1 << ". " << interfaces[i] << endl;
//    }
    cin >> ifs;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
//    if (ifs > interfaces->length()) {
//        cout << "Invalid interface choice" << endl;
//        throw ;
//    }
    return (int)ifs;
}

string get_host_from_user() {
    string host;

    cout << "Enter the IP address of the destination computer: ";
    getline(cin, host);
    if (host.length() > MAX_HOST_NAME) {
        cout << "Invalid host - should be no longer than " << MAX_HOST_NAME << " characters" << endl;
    }
    return host;
}

ComputerID query_connection(){
    int ifs = get_interface_from_user();
    string host = get_host_from_user();
    return ComputerID(host, ifs);
}

TunnelRequest* list_cmd_handler() { //TODO: check where to initialize the interface
    return new ListInterfaceRequest();
}


TunnelRequest* communicate_cmd_handler() {
    ComputerID id = query_connection();
    cout << "Enter command: " << endl;
    string command;
    getline(cin, command);
    return new CommunicateConnectionRequest(id, command);
}

TunnelRequest* query_cmd_handler() {
    int ifs = get_interface_from_user();
    return new QueryInterfaceRequest(ifs);
}

TunnelRequest* connect_cmd_handler() {
    ComputerID id = query_connection();
    return new OpenConnectionRequest(id);
}

TunnelRequest* disconnect_cmd_handler() {
    ComputerID id = query_connection();
    return new CloseConnectionRequest(id);
}


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

#define MENU_ITEM(item_num, item_str, handler) (char*)item_str,
char *options_str[] = {
        MENU_TABLE
};
#undef MENU_ITEM

typedef TunnelRequest* (*cmd_handler)(void);

#define MENU_ITEM(item_num, item_str, handler) handler,
cmd_handler cmd_handlers[] = {
        MENU_TABLE
};
#undef MENU_ITEM

int safe_get_number(int min_num, int max_num) {
    int good_num;
    cout << "Please enter a valid integer" << endl;
    for(;;) {
        if (cin >> good_num) {
            if (min_num < good_num && good_num < max_num)
                break;
        }
        cout << "Please enter a valid integer" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return good_num;
}

TunnelRequest* tunnel_menu() {
    int choice = 0;

    while (true) {
        cout << "What do you want to do:" << endl;
        for (int i = 0; i < NUM_ITEMS; i++) {
            cout << i + 1 << ". " << options_str[i] << endl;
        }
        choice = safe_get_number(0, NUM_ITEMS + 1);
        // Ignore to the end of file
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        choice -= 1;
        if (EXIT == choice) {
            if (verify_exit_choice()) {
                return NULL;
            }
            continue;
        }
        if (choice >= NUM_ITEMS || choice < 0) {
            if (cin.fail()) {
                cin.clear();
                cout << "Very bad input!" << endl;
            }
            cout << "Bad choice: should enter a valid option" << endl;
            continue;
        }
        return cmd_handlers[choice]();
    }
}


void open_user_session(ClientConnection &conn, OpenConnectionRequest *open_cmd) {
    bool should_continue;

    TunnelRequest *cur_cmd = NULL;
    ComputerID current_id = open_cmd->get_conn_id();
    string command;
    should_continue = true;

    conn.send_and_receive(open_cmd);
    while (should_continue) {
        cout << current_id << ": ";
        getline(cin, command);
        if ("exit" == command || "quit" == command) {
            cur_cmd = new CloseConnectionRequest(current_id);
            should_continue = false;
        } else {
            cur_cmd = new CommunicateConnectionRequest(current_id, command);
        }
        conn.send_and_receive(cur_cmd);
    }
}

void run(string server_name, string port) {
    ClientConnection conn(server_name, port);

    while (true) {
        TunnelRequest *cmd = tunnel_menu();
        if (NULL == cmd) {
            cout << "Bye bye" << endl;
            break;
        }
        if (OPEN_CONNECTION == cmd->get_type()) {
            open_user_session(conn, (OpenConnectionRequest*)cmd);
        } else {
            conn.send_and_receive(cmd);
        }
    }
}

void change_console_echo_mode() {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
}

int __cdecl main(int argc, char **argv) {
    // Validate the parameters
    if (argc != 3) {
        printf("usage: %s <server_name> <port>\n", argv[0]);
        return 1;
    }
//    change_console_echo_mode();
    run(argv[1], argv[2]);
//    int r = safe_get_number(2, 6);
    return 0;
}

int __cdecl main1(int argc, char **argv) {
    byte_t buf[MAX_BUFFER_SIZE];

    ClientConnection conn("localhost", "7777");

    CommunicateConnectionRequest *cmd = new CommunicateConnectionRequest(ComputerID("localhost", 2), "bla bla");
    int buf_size = cmd->serialize(buf, MAX_BUFFER_SIZE);
    conn.send_buffer((char*)buf, (size_t)buf_size);
//    TunnelRequest *pcmd = parse_command(buf, buf_size);
//    cout << pcmd->get_type() << endl;

    return 0;
}
