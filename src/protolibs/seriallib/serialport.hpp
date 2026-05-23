
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
#endif // _WIN32

class SerialPort {
    public:
        enum Error {
            NONE,
            OPEN
        };

        enum Baud {
            BAUD_9600 = 9600,
            BAUD_19200 = 19200,
            BAUD_38400 = 38400,
            BAUD_57600 = 57600,
            BAUD_115200 = 115200
        };



        // basic functions
        SerialPort();
        ~SerialPort();

        SerialPort::Error open(const char* name, uint32_t baudrate);

        SerialPort::Error close();

        SerialPort::Error configure();



    private:
        
        int _fd = -1;

        SerialPort::Error _set_baudrate(uint32_t baud);

};


