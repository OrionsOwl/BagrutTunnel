#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include "Server.h"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

int __cdecl main(void) {
    int res = 0;
    int count = 0;
    int buflen = 0;
    char recvbuf[DEFAULT_BUFLEN];
    Server conn(DEFAULT_PORT);

    for (count = 0; count < 5; count++) {
        conn.wait_for_client();
        res = conn.recv_data(recvbuf, DEFAULT_BUFLEN, &buflen);
        if (0 > res) {
            conn.close_client();
            continue;
        }
        res = conn.send_buffer(recvbuf, buflen);
        if (0 > res) {
            conn.close_client();
        }
    }

    return 0;
}