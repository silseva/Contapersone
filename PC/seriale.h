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

#ifndef SERIALE_H
#define SERIALE_H

#include <cstdio>
#include <cstdint>
#include <stdexcept>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

class Seriale
{
public:
	Seriale(const char* name);
	~Seriale();
	uint32_t requestValue();
	
private:
	int serialFd;
};

#endif