/*
 * Simple and stupid serial port driver used to poll a 32 bit
 * unsigned value from the MCU
 * Copyright (C) 2017  Silvano Seva silseva@fastwebnet.it
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "serialport.h"

using namespace std;

SerialPort::SerialPort(const char* name)
{
    serialFd = open(name, O_RDWR | O_NOCTTY | O_NDELAY);
    fcntl(serialFd, F_SETFL, 0);

    if (serialFd == -1)
    {
        throw runtime_error("Unable to open the port");
    }

    struct termios tty;
    if (tcgetattr(serialFd, &tty) != 0)
    {
        throw runtime_error("Tcgetattr failed");
    }

    // 9600 baud both for TX and RX
    cfsetospeed(&tty, (speed_t)B9600);
    cfsetispeed(&tty, (speed_t)B9600);

    tty.c_cflag &= ~PARENB;  // Make 8n1
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    tty.c_cflag &= ~CRTSCTS;  // no flow control
    tty.c_cc[VMIN]  = 1;      // read at least 1 byte
    tty.c_cc[VTIME] = 0.5;    // 500 ms timeout

    cfmakeraw(&tty);

    // flush both TX and RX
    tcflush(serialFd, TCIFLUSH);

    if (tcsetattr(serialFd, TCSANOW, &tty) != 0)
    {
        throw runtime_error("Tcsetattr failed");
    }
}

SerialPort::~SerialPort()
{
    tcflush(serialFd, TCIFLUSH);
    close(serialFd);
}

uint32_t SerialPort::requestValue()
{
    // send command
    uint8_t cmd[] = {0x55, 0xDD};
    int n = write(serialFd, cmd, 2);
    if (n < 0)
        return 0;

    // HACK: because MCU and PC have the same endianness, this trick
    // can be done. We cast a pointer to uint32_t to pointer to uint8_t
    // and the we tell the system to read 4 bytes, automagically placing them
    // in the right order
    uint32_t value = 0;
    n = read(serialFd, reinterpret_cast<uint8_t*>(&value), 4);
    if (n < 0)
        return 0;

    tcflush(serialFd, TCIFLUSH);

    return value;
}
