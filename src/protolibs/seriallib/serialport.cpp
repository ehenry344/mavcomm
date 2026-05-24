#include "serialport.hpp"

// add a function to get the baudrate values for linux / windows respectively


SerialPort::Error SerialPort::open(const char* port_name) {
    // compile time evaluation
    constexpr int flags = O_RDWR | O_NOCTTY | O_SYNC;

    const int ret = ::open(port_name, flags);

    if (ret < 0) {
        // check the errno
#ifdef _WIN32
        return Error::NOT_OPEN; // TODO: implement for windows
#else
        switch (errno) {
            // TODO: finish these later
            case ENOENT:
                return Error::NOT_FOUND;
            case EBUSY:
                return Error::INUSE;
            default:
                return Error::NOT_OPEN;
        }
#endif // _WIN32
    }

    _handle = ret;
    
    return Error::NONE;
}

SerialPort::Error SerialPort::close() {
    if (_handle >= 0) {
        ::close(_handle);
        _handle = -1;
    }

    return Error::NONE;
}

SerialPort::Error SerialPort::configure() {
    if (_handle < 0) {
        return Error::NOT_OPEN;
    }

#ifdef _WIN32
    // TODO: implement for windows
#else
    // POSIX based port config
    struct termios options;
    int ret = tcgetattr(_handle, &options);

    if (ret != 0) {
        // TODO: add more specific error handling
        return Error::CONFIG;
    }
#endif // _WIN32

    return Error::NONE;
}

SerialPort::Error SerialPort::write(const uint8_t* buf, 
                                    std::size_t buflen, 
                                    std::size_t& written) {
    if (_handle < 0) {
        return Error::NOT_OPEN;
    }

    int ret;
#ifdef _WIN32
    // TODO: implement for windows
#else
    ret = ::write(_handle, buf, buflen);

    if (ret < 0) {
        // TODO: add error handling for this
        return Error::WRITE;
    }

    written = static_cast<std::size_t>(ret);
#endif // _WIN32

    return Error::NONE;
}

SerialPort::Error SerialPort::read(uint8_t* buf, 
                                   std::size_t buflen, 
                                   std::size_t& read_len) {
    if (_handle < 0) {
        return Error::NOT_OPEN;
    }

    int ret;
#ifdef _WIN32

#else
    ret = ::read(_handle, buf, buflen);

    if (ret < 0) {
        // TODO: add error handling for this
        return Error::WRITE;
    }

    read_len = static_cast<std::size_t>(ret);
#endif // _WIN32

    return Error::NONE;
}