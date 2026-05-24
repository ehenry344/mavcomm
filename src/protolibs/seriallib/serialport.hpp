
// Cross Platform Serial Port Interface

#pragma once

// stdlib
#include <cstdint>
#include <string>
#include <stdexcept>

#include <string.h>
#include <errno.h>

#ifdef _WIN32

#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#endif // _WIN32

class SerialPort {
    public:
        enum Error {
            NONE,
            NOT_OPEN,
            NOT_FOUND,
            PERMISSION,
            INUSE,
            CONFIG,
            WRITE,
        };

        enum Baud {
            BAUD_9600,
            BAUD_19200,
            BAUD_38400,
            BAUD_57600,
            BAUD_115200,
            BAUD_230400,
            BAUD_460800,
            BAUD_921600
        };



        // basic functions
        SerialPort();
        ~SerialPort();

        SerialPort::Error open(const char* port_name);

        SerialPort::Error close();

        SerialPort::Error configure();

        SerialPort::Error write(const uint8_t* buf, 
                                std::size_t buflen, 
                                std::size_t& written);

        SerialPort::Error read(uint8_t* buf, 
                               std::size_t buflen, 
                               std::size_t& read_len);



    private:

#ifdef _WIN32
        HANDLE _handle = INVALID_HANDLE_VALUE;
#else
        int _handle = -1;
#endif // _WIN32

        SerialPort::Error _set_baudrate(uint32_t baud);

};


