#define WIN32_LEAN_AND_MEAN

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "Client.h"


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
#define DEFAULT_HOST "localhost"

char ip[30];
int current_ifs_num(0);


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

void enter_ip(){//char*
    //char *current_ip;
    printf("Enter the IP address of the destination computer.\n");
    scanf("%s",&ip);
    printf("ip: %s",ip);
    //ip=current_ip;
}

typedef struct interface_s {
    char inners[5];
    //int inners[2];
    int ifs_num;

} interface_t;
//typedef struct ETH1{int inner1=1;}eth1_t;
//should be a #define?

//typedef struct ETH2{int inner2=2; int inner3=3;}eth2_t;

typedef union INTERFACES{
    interface_s ifs1;
    interface_s ifs2;
} interfaces_t;


int list_cmd_handler() { //TODO: check where to initialize the interface
    //interfaces_t->ifs1->inners[0]={"inner1"};
    // interfaces_t.ifs2->inners[0]={"inner2"};
    // interfaces_t.ifs2->inners[6]={"inner3"};
    //interfaces_t.ifs1.inners[0]={1};

    INTERFACES interfaces_t;
    interfaces_t.ifs1.ifs_num=1;
    interfaces_t.ifs2.ifs_num=2;
    interfaces_t.ifs1.inners[0]=0;
    interfaces_t.ifs2.inners[0]=0;
    interfaces_t.ifs2.inners[1]=0;

    printf("Performing interface list*******************\n");
    printf("Which interface do you choice?\n");

    int choice = 0;
    int ineers_choice=0;
    for(int i=1; i<3;i++)
    {
        printf("%d %s %d\n", i,"eth",i);
    }
    scanf("%d", &choice);

    switch(choice) {
        case 1  :{
            printf("Do you want to connect to: inner1?\n 1.Yes\n 2.No\n");
            scanf("%d", &ineers_choice);
            //interfaces_t.ifs1.ifs_num= choice;
            switch(ineers_choice) {
                case 1:{
                    printf("Interface number=%d\n",interfaces_t.ifs1.ifs_num);
                    current_ifs_num= interfaces_t.ifs1.ifs_num;
                    interfaces_t.ifs1.inners[0]=1;
                }

                case 2  :{
                    printf("OK");//what does it need to do?
                }
            }
        }

        case 2  :{
            printf("Do you want to connect to: 1.inner2?\n 2.inner3\n 3.Non of the above\n");
            scanf("%d", &ineers_choice);
            //interfaces_t.ifs2.ifs_num= choice;
            switch(ineers_choice) {
                case 1:{
                    printf("%d\n",interfaces_t.ifs2.ifs_num);
                    current_ifs_num= interfaces_t.ifs2.ifs_num;
                    interfaces_t.ifs2.inners[0]=1;
                }

                case 2:{
                    printf("%d\n",interfaces_t.ifs2.ifs_num);
                    current_ifs_num= interfaces_t.ifs2.ifs_num;
                    interfaces_t.ifs2.inners[1]=1;
                }

                case 3  :{
                    printf("OK\n");//what does it need to do?
                }

            }
        }

    }
    return 0;
}

int communicate_cmd_handler() {
    char msg[20];
    printf("Asking the machine for something\n");
    scanf("%s",&msg);
    printf("Your msg was: %s\n",msg);

    // Waiting time
    // print results
    return 0;
}

int query_cmd_handler() {
    printf("Performing query interface...\n"); //TODO: interface
    // Waiting time
    // print results
    return 0;
}

int connect_cmd_handler() {
    //build
    printf("Connecting...\n");
    // Waiting time
    // raise exception!!!
    //ip=
    enter_ip();
    printf("ip: %s. interface number=%d.\n",ip, current_ifs_num);
    printf("Connection created!\n What do you want to do?\n");
    for (int i = 0; i <7 ; i ++) {//NUM_ITEMS
        if(3 == i || 4 == i)
            printf("%d.\n", i+1);
        //         printf("%d. %s.\n", i+1, options_str[i]);
    }
    int choice =0;
    scanf("%d", &choice);
//    switch(choice) {
//        case COMMUNICATE_IFS  :
//            communicate_cmd_handler();
//        case QUERY_IFS  :
//            query_cmd_handler();
//    }

    return 0;
}//char* &ip, int ifs_num

int disconnect_cmd_handler() {
    // Waiting time
    // raise exception!!!
    printf("Are you sure you want to disconnect?\n 1.No\n 2.Yes\n");
    int choice=0;
    scanf("%d", &choice);
    // raise exception!!!
    switch(choice) {
        case 1  :
            break;
        case 2  :
            printf("Disconnecting from ip: %s, interface num: %d\n", ip, current_ifs_num); //TODO: interface
            list_cmd_handler();
    }

    return 0;
}


int exit_cmd_handler() {
    printf("Are you sure you want to exit?\n 1.No\n 2.Yes\n");
    int choice=0;
    scanf("%d", &choice);
    // raise exception!!!
    switch(choice) {
        case 1  :
            return 0;
        case 2  :
           // list_cmd_handler();
            return 1;
    }

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

int tunnel_menu() {
    int i = 0;
    int choice = 0;
    int should_continue = 1;

    enter_ip();

    while (should_continue) {
        printf("What do you want to do:\n");
        for (i = 0; i < NUM_ITEMS; i ++) {
            printf("%d. %s.\n", i+1, options_str[i]);
        }
        scanf("%d", &choice);
        choice -= 1;
        if (EXIT == choice) {
            if (1== exit_cmd_handler()){
                should_continue = 0;
            }
            continue;
        }
        if (choice >= NUM_ITEMS || choice < 0) {
            printf("Bad choice: should enter a valid option\n");
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