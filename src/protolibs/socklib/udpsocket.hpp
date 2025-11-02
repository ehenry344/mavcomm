
// Cross Platform UDP Socket Wrapper

#pragma once

// stdlib
#include <cstdio>
#include <cstdint>
#include <string>
#include <stdexcept>

#include <string.h>
#include <errno.h>

#ifdef _WIN32
    #include <ws2tcpip.h>
    #include <winsock2.h>

    #pragma comment(lib, "Ws2_32.lib")
#else
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <unistd.h>
#endif // _WIN32

struct sockinfo_t {
    std::string ip_addr;
    uint16_t port;
};

class UDPSocket {
    public:
        // return codes
        enum Error {
            None,
            AlreadyBound,
            AddrInvalid,
            AddrFamilyInvalid,
            BindFail,
            CloseFail,
            SockNotBound,
            SetSockOptFail,
        };
 
        UDPSocket();
        ~UDPSocket();

        uint8_t bind(const sockinfo_t& sockinfo);
        uint8_t close();
        uint8_t set_recv_timeout(unsigned int timeout_ms);

        uint8_t sendto(const sockinfo_t& dst, uint8_t* buf, std::size_t buf_len);



    private:
        struct sockinfo_t _sockinfo;

        bool _is_bound = false;

        int _sock_fd;
        struct sockaddr_in _sock_addr;
};


