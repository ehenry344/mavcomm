
// Cross Platform UDP Socket Wrapper

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

typedef struct {
    char ip[INET_ADDRSTRLEN];
    uint16_t port;
} sockinfo_t;

class UDPSocket {
    public:
        // return codes
        enum Error {
            NONE,
            WSASTARTUP,
            BOUND,
            INVALID_ADDR,
            INVALID_ADDR_FAMILY,
            BIND,
            CLOSE,
            NOTBOUND,
            SETSOCKOPT,
            SEND,
            RECV,
        };
 
        UDPSocket();
        ~UDPSocket();

        uint8_t bind(const sockinfo_t& sockinfo);

        uint8_t close();

        uint8_t sendto(const sockinfo_t& dst, const uint8_t* buf, std::size_t buf_len);
        uint8_t recvfrom(uint8_t* buf, 
                         std::size_t buf_len, 
                         std::size_t& recv_len, 
                         sockinfo_t& src);
        
        uint8_t set_recv_timeout(std::size_t timeout_ms);

    private:

        // socket file descriptor
        int _sock_fd;

        // internal sockaddr
        struct sockaddr_in _sock_addr;

        // socket location
        sockinfo_t _sock_info;

        // binding status
        bool _bound = false;

        // internal functions
};


