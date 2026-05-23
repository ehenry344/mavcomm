#include "serialport.hpp"

SerialPort::Error SerialPort::open(const char* name, uint32_t baudrate) {
    _fd = ::open(name, O_RDWR | O_NOCTTY | O_SYNC);
    if (_fd < 0) {
        return Error::OPEN;
    }
    
    return Error::NONE;
}

SerialPort::Error SerialPort::close() {
    if (_fd >= 0) {
        ::close(_fd);
        _fd = -1;
    }

    return Error::NONE;
}