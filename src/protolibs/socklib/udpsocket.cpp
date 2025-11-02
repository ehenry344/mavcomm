// cross platform UDP socket implementation

#include "udpsocket.hpp"

UDPSocket::UDPSocket() {
    #ifdef _WIN32
        // WSAStartup : initialize internal socket backend
        WSADATA wsa_data;
        int ret = ::WSAStartup(MAKEWORD(2, 2), &wsa_data);

        // eventually call WSACleanup when we are done with this socket, otherwise WSA will continue being loaded in the background

        if (ret != 0) {
            throw std::runtime_error("UDPSocket::UDPSocket failed on WSAStartup");
        }
    #endif
}

uint8_t UDPSocket::bind(const sockinfo_t& sockinfo) {
    if (_is_bound) {
        return Error::AlreadyBound;
    }

    _sockinfo.ip_addr = sockinfo.ip_addr;
    _sockinfo.port = sockinfo.port;

    // create new socket descriptor
    int ret = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (ret < 0) {
        throw std::runtime_error("UDPSocket::UDPSocket, socket creation failed");
    }
    _sock_fd = ret;

    // clear socket addr
    memset(&_sock_addr, 0, sizeof(_sock_addr));

    _sock_addr.sin_family = AF_INET;
    _sock_addr.sin_port = ::htons(_sockinfo.port);

    ret = ::inet_pton(AF_INET, _sockinfo.ip_addr.c_str(), &_sock_addr.sin_addr);
    if (ret != 1) {
        if (ret == 0) {
            return Error::AddrInvalid;
        }
        return Error::AddrFamilyInvalid;
    }

    if (::bind(_sock_fd, (struct sockaddr*)&_sock_addr, sizeof(_sock_addr)) != 0) {
        return Error::BindFail;
    }

    _is_bound = true;

    return Error::None;
}

uint8_t UDPSocket::close() {
    if (_is_bound) {
        int ret;
        #ifdef _WIN32
            ret = ::closesocket(_sock_fd);
        #else
            ret = ::close(_sock_fd);
        #endif

        if (ret < 0) {
            return Error::CloseFail;
        }

        _is_bound = false;
    }

    return Error::None;
}

// receive timeout for recvfrom calls
uint8_t UDPSocket::set_recv_timeout(unsigned int timeout_ms) {
    if (!_is_bound) {
        return Error::SockNotBound;
    }

    int ret;
    #ifdef WIN32
        ret = ::setsockopt(
            _sock_fd,
            SOL_SOCKET,
            SO_RCVTIMEO,
            (const char*)&timeout_ms,
            sizeof(timeout_ms)
        );
    #else   
        struct timeval recv_to;
        recv_to.tv_sec = 0;
        recv_to.tv_usec = (timeout_ms * 1000);

        ret = ::setsockopt(
            _sock_fd,
            SOL_SOCKET,
            SO_RCVTIMEO,
            &recv_to,
            sizeof(recv_to)
        );
    #endif

    if (ret != 0) {
        return Error::SetSockOptFail;
    }

    return Error::None;
}

uint8_t UDPSocket::sendto(const sockinfo_t& dst, uint8_t* buf, std::size_t buf_len) {
    
}

