#undef UNICODE

#define WIN32_LEAN_AND_MEAN

//#include <stdio.h>
//#include "ServerConnection.h"
#include "TunnelHandler.h"

//#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "7777"

int __cdecl main(void) {
    TunnelHandler tunnel((char*)DEFAULT_PORT);
    tunnel.run();
    return 0;
}