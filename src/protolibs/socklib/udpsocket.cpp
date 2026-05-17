// udpsocket.cpp
// this file implements a cross platform object oriented UDP socket interface

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
    if (_bound) {
        return Error::BOUND;
    }

    strcpy(_sock_info.ip, sockinfo.ip);
    _sock_info.port = sockinfo.port;

    // create new socket descriptor
    int ret = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (ret < 0) {
        return Error::BIND;
    }
    _sock_fd = ret;

    // clear socket addr
    memset(&_sock_addr, 0, sizeof(_sock_addr));

    _sock_addr.sin_family = AF_INET;
    _sock_addr.sin_port = ::htons(_sock_info.port);

    ret = ::inet_pton(AF_INET, _sock_info.ip, &_sock_addr.sin_addr);
    if (ret != 1) {
        close(); // socket can get leaked here
        if (ret == 0) {
            return Error::INVALID_ADDR;
        }
        return Error::INVALID_ADDR_FAMILY;
    }

    if (::bind(_sock_fd, (struct sockaddr*)&_sock_addr, sizeof(_sock_addr)) != 0) {
        return Error::BIND;
    }

    _bound = true;

    return Error::NONE;
}

uint8_t UDPSocket::close() {
    if (_bound) {
        int ret;
#ifdef _WIN32
        ret = ::closesocket(_sock_fd);
#else
        ret = ::close(_sock_fd);
#endif

        if (ret < 0) {
            return Error::CLOSE;
        }

        _bound = false;
    }

    return Error::NONE;
}

// receive timeout for recvfrom calls
uint8_t UDPSocket::set_recv_timeout(std::size_t timeout_ms) {
    if (!_bound) {
        return Error::NOTBOUND;
    }

    int ret;
#ifdef _WIN32
    ret = ::setsockopt(
        _sock_fd,
        SOL_SOCKET,
        SO_RCVTIMEO,
        (const char*)&timeout_ms,
        sizeof(timeout_ms)
    );
#else
    // Linux / POSIX based systems require a timeval structure
    // instead of just providing the number itself
    struct timeval recv_to;
    recv_to.tv_sec = timeout_ms / 1000;
    recv_to.tv_usec = (timeout_ms % 1000) * 1000;

    ret = ::setsockopt(
        _sock_fd,
        SOL_SOCKET,
        SO_RCVTIMEO,
        &recv_to,
        sizeof(recv_to)
    );
#endif

    if (ret != 0) {
        return Error::SETSOCKOPT;
    }

    return Error::NONE;
}

uint8_t UDPSocket::sendto(const sockinfo_t& dst, const uint8_t* buf, std::size_t buf_len) {
    if (!_bound) {
        return Error::NOTBOUND;
    }

    // temporarily initialize sockaddr structure to send the data
    struct sockaddr_in dst_sa {};
    dst_sa.sin_family = AF_INET;
    dst_sa.sin_port = htons(dst.port);
    
    int ret = ::inet_pton(AF_INET, dst.ip, &dst_sa.sin_addr);
    if (ret != 1) {
        if (ret == 0) {
            return Error::INVALID_ADDR;
        }
        return Error::INVALID_ADDR_FAMILY;
    }

    ret = ::sendto(
        _sock_fd,
        (const char*)buf,
        buf_len,
        0,
        (const sockaddr*)&dst_sa,
        sizeof(dst_sa)
    );

    if (ret < 0 || ret != buf_len) {
        return Error::SEND;
    }

    return Error::NONE;
}

uint8_t UDPSocket::recvfrom(uint8_t* buf,
                            std::size_t buf_len,
                            std::size_t& recv_len, 
                            sockinfo_t& src) {
    if (!_bound) {
        return Error::NOTBOUND;
    }

    sockaddr_in src_sa {};
    socklen_t src_sa_len = sizeof(src_sa);

    int ret = ::recvfrom(
        _sock_fd,
        (char*)buf,
        buf_len,
        0,
        (sockaddr*)&src_sa,
        &src_sa_len
    );

    if (ret < 0) {
        return Error::RECV;
    }
    
    // parse the source from src_sa
    if (!::inet_ntop(AF_INET, &src_sa.sin_addr, src.ip, INET_ADDRSTRLEN)) {
        return Error::RECV;
    }

    src.port = ntohs(src_sa.sin_port);

    recv_len = static_cast<std::size_t>(ret);

    return Error::NONE;
}

