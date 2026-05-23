
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



        // basic functions
        SerialPort();
        ~SerialPort();

        SerialPort::Error open(const char* name, uint32_t baudrate);

        SerialPort::Error close();




    private:
        
        int _fd = -1;

};


