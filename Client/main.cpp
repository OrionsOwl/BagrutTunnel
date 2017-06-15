#define WIN32_LEAN_AND_MEAN

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
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



//#define INTERFACES_TABLE \
    INTERFACES_TABLE(ETH1, "eth1") \
    INTERFACES_TABLE(ETH2, "eth2") \
    INTERFACES_TABLE(interface_NUM_ITEMS, "")

//#define INTERFACES_ITEM(interface_item_num, interface_item_lan) interface_item_num,
//typedef enum interface_e{
//    INTERFACES_TABLE
//}interface_t;
//#undef INTERFACES_ITEM

//#define INTERFACES_ITEM(interface_item_num, interface_item_lan) interface_item_lan,
//typedef union lan_u{
//    INTERFACES_TABLE;
//}lan_t;
//#undef INTERFACES_ITEM


//union
//{
//int a;
//const char* p;
//};
//a = 1;
//p = "Jennifer";

typedef struct ETH1{int inner1;};
//should be a #define?

typedef struct ETH2{int inner2; int inner3;};

typedef union INTERFACES{
    ETH1;
    ETH2;

}interfaces_t;

int list_cmd_handler() {
    printf("Performing interface list*******************\n");
    printf("Which interface do you choice?\n");

    for(int i=1; i<3;i++)
    {
        printf(i,"eth",i);
    }
    scanf("%d", &choice);

    switch(&choice) {
        case 1  :
            printf("Do you want to connect to: inner1?\n 1.Yes\n 2.No\n");
            scanf("%d", &choice);
            switch(&choice) {
                case 1:
                    printf(ETH1.inner1);
                case 2  :
                    conntinue; //what does it need to do?
            }

        case 2  :
            printf("Do you want to connect to: 1.inner2?\n 2.inner3\n 2.Non of the above\n");
            scanf("%d", &choice);
            switch(&choice) {
                case 1:
                    printf(ETH2.inner2);
                case 2:
                    printf(ETH2.inner3);
                case 3  :
                    conntinue; //what does it need to do?
            }
    }
   return 0;
}

int connect_cmd_handler() {
    //build
    printf("Connecting...\n");
    // Waiting time
    // raise exception!!!
    printf("Connection created!\n What do you want to do?\n");
    for (i = 0; i < NUM_ITEMS; i ++) {
        if(3== i || 4==i)
        printf("%d. %s.\n", i+1, options_str[i]);
    }
    scanf("%d", &choice);
    switch(&choice) {
        case COMMUNICATE_IFS  :
            communicate_cmd_handler();
        case QUERY_IFS  :
            query_cmd_handler();
    }

    return 0;
}

int disconnect_cmd_handler() {
    printf("Disconnecting...\n");
    // Waiting time
    // raise exception!!!
    printf("Are you sure you want to disconnect?\n 1.No\n 2.Yes\n");
    scanf("%d", &choice);
    // raise exception!!!
    switch(&choice) {
        case 1  :
            conntinue;
        case 2  :
            list_cmd_handler();
    }

    return 0;
}

int communicate_cmd_handler() {
    printf("Asking the machine for something\n");



    // Waiting time
    // print results
    return 0;
}

int query_cmd_handler() {
    printf("Performing query interface...\n");
    // Waiting time
    // print results
    return 0;
}

int exit_cmd_handler() {
    printf("Are you sure you want to exit?\n 1.No\n 2.Yes\n");
    scanf("%d", &choice);
    // raise exception!!!
    switch(&choice) {
        case 1  :
            conntinue;
        case 2  :
            list_cmd_handler();
    }

    return 0;
}

#define MENU_TABLE \
    MENU_ITEM(IFS_LIST, "interface list", list_cmd_handler) \
    MENU_ITEM(CONNECT_IFS, "connect to machine", connect_cmd_handler) \
    MENU_ITEM(DISCONNECT_IFS, "disconnect from machine", disconnect_cmd_handler) \
    MENU_ITEM(COMMUNICATE_IFS, "talk to machine", communicate_cmd_handler) \
    MENU_ITEM(QUERY_IFS, "query interface", query_cmd_handler) \
    MENU_ITEM(EXIT, "exit", exit_cmd_handler) \
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

    tunnel_menu();
//    test_client_connection();
}