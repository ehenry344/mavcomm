#pragma once

// stdlib
#include <cstdio>
#include <string>
#include <string.h>

#ifdef _WIN32
    #include <ws2tcpip.h>
    #include <winsock2.h>

    #pragma comment(lib, "Ws2_32.lib")
#else
    
#endif // _WIN32
