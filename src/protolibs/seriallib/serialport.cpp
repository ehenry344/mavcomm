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

SerialPort::Error SerialPort::set_baudrate(unsigned int baudrate) {
    _baudrate = baudrate;
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

    // configure control flags
    options.c_cflag |= (CLOCAL | CREAD); // enable receiver, ignore modem control lines
    options.c_cflag &= ~CSIZE; // clear current char size mask
    options.c_cflag |= CS8; // set 8 data bits
    options.c_cflag &= ~PARENB; // disable parity
    options.c_cflag &= ~CSTOPB; // set 1 stop bit
    options.c_cflag &= ~CRTSCTS; // disable hardware flow control
    
    // configure local mode flags
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // raw input mode

    // confgure input flags
    constexpr tcflag_t flow_ctrl_flags = ~(IXON | IXOFF | IXANY);

    options.c_iflag &= flow_ctrl_flags; // disable software flow control

    constexpr tcflag_t input_processing_flags = ~(IGNBRK | BRKINT | PARMRK | \
                                                  ISTRIP | INLCR | IGNCR |  \
                                                  ICRNL);
    
    options.c_iflag &= input_processing_flags; // disable input processing

    // configure output flags
    options.c_oflag &= ~OPOST; 
    options.c_oflag &= ~ONLCR; // disable output processing

    // set baudrate
    speed_t baudrate;
    switch (_baudrate) {
        case 9600:
            baudrate = B9600;
            break;
        case 19200:
            baudrate = B19200;
            break;
        case 38400:
            baudrate = B38400;
            break;
        case 57600:
            baudrate = B57600;
            break;
        case 115200:
            baudrate = B115200;
            break;
        default:
            return Error::CONFIG; // unsupported baudrate
    }

    ret = cfsetispeed(&options, baudrate);
    if (ret != 0) {
        return Error::CONFIG;
    }

    ret = cfsetospeed(&options, baudrate);
    if (ret != 0) {
        return Error::CONFIG;
    }

    // save changes to termios struct
    ret = tcsetattr(_handle, TCSANOW, &options);
    if (ret != 0) {
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