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

SerialPort::Error SerialPort::configure() {
    if (_fd < 0) {
        return Error::OPEN;
    }

    // this function enables all of the various flags
    // for the serial port





    return Error::NONE;
}

SerialPort::Error SerialPort::_set_baudrate(uint32_t baud) {
    // set the baudrate for the port handle
#ifdef _WIN32
    // TODO: implement for windows
#else
    
#endif // _WIN32
    return Error::NONE;
}