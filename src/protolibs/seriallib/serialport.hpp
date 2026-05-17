
// Cross Platform Serial Port Interface

#pragma once

// stdlib
#include <cstdint>
#include <string>
#include <stdexcept>

#include <string.h>
#include <errno.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#endif // _WIN32

class SerialPort {
    public:


    private:
    
};


