#define WIN32_LEAN_AND_MEAN

#include <stdlib.h>
#include <stdio.h>
#include "Client.h"


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
#define DEFAULT_HOST "localhost"

int test_client_connection() {
    Client conn(DEFAULT_HOST, DEFAULT_PORT);
    char *sendbuf = (char *) "this is a test";
    int res;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    // Send an initial buffer
    res = conn.send_buffer(sendbuf, (uint16_t) strlen(sendbuf));
    if (-1 == res) {
        return 1;
    }

    printf("Bytes Sent: %d\n", res);

    res = conn.recv_data(recvbuf, recvbuflen, &recvbuflen);

    if (res == 0) {
        printf("Received bytes: %d, '%s'\n", recvbuflen, recvbuf);
    } else {
        printf("Error: %d", res);
    }

    return 0;
}

int list_cmd_handler() {
    printf("Performing interface list*******************\n");
    // Waiting time
    // print results
    return 0;
}

#define MENU_TABLE \
    MENU_ITEM(IFS_LIST, "interface list", list_cmd_handler) \
    MENU_ITEM(CONNECT_IFS, "connect to machine", NULL) \
    MENU_ITEM(DISCONNECT_IFS, "disconnect from machine", NULL) \
    MENU_ITEM(COMMUNICATE_IFS, "talk to machine", NULL) \
    MENU_ITEM(QUERY_IFS, "query interface", NULL) \
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

typedef int(*cmd_handler)(void);

#define MENU_ITEM(item_num, item_str, handler) handler,
cmd_handler cmd_handlers[] = {
        MENU_TABLE
};
#undef MENU_ITEM


int tunnel_menu() {
    int i = 0;
    int choice = 0;
    int should_continue = 1;

    while (should_continue) {
        printf("What do you want to do:\n");
        for (i = 0; i < NUM_ITEMS; i ++) {
            printf("%d. %s.\n", i+1, options_str[i]);
        }
        scanf("%d", &choice);
        choice -= 1;
        if (EXIT == choice) {
            should_continue = 0;
            continue;
        }
        if (choice >= NUM_ITEMS) {
            printf("Bad choince: should enter a valid option\n");
            continue;
        }
        if (NULL != cmd_handlers[choice]) {
            cmd_handlers[choice]();
        }
    }
    return 0;
}

int __cdecl main(int argc, char **argv) {
    // Validate the parameters
//    if (argc != 3) {
//        printf("usage: %s <server_name> <port>\n", argv[0]);
//        return 1;
//    }

//    argv[1] = server_name
//    argv[2] = port

//    tunnel_menu();
    test_client_connection();
}