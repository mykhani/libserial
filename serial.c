/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2016 Yasir Khan
 * Email: yasir_electronics@yahoo.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "serial.h"
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int serial_setup(int fd, int baudrate)
{
	struct termios serial_config;
	/* zeroize configuration */
	memset(&serial_config, 0, sizeof(serial_config));
	/* get current serial attr */
	if (tcgetattr(fd, &serial_config)) {
		perror("Unable to get serial attributes: %s\r\n", strerror(errno));
		close(fd);
		return -ENODEV;
	}
	else {
		serial_config.c_iflag &= ~(INLCR | ICRNL);
		serial_config.c_iflag |= IGNPAR | IGNBRK;
		serial_config.c_oflag &= ~(OPOST | ONLCR | OCRNL);
		serial_config.c_cflag &= ~(PARENB | PARODD | CSTOPB | CSIZE | CRTSCTS);
		serial_config.c_cflag |= CLOCAL | CREAD | CS8;
		serial_config.c_lflag &= ~(ICANON | ISIG | ECHO);
		/* Set the desired baudrate */
		cfsetospeed(&serial_config, B115200);
		cfsetospeed(&serial_config, B115200);
		/* data wait time-out */
		serial_config.c_cc[VTIME] = 1;
		/* Minimum data bytes available before read is successful */
		serial_config.c_cc[VMIN] = 0; 
		/* set attributes */
		if (tcsetattr(fd, TCSANOW, &serial_config)) {
			perror("Unable to set serial attributes: %s\r\n", strerror(errno));
			close(fd);
		}
	}
	return 0;
}

int serial_write(int fd, const char *data)
{
	return write(fd, data, strlen(data));
}

int serial_read(int fd)
{
}
/* open the serial device, make sure it is not being used by any other program */
int serial_open(const char *name)
{
	int serial_fd = open(name, O_RDWR | O_NOCTTY);
	if (serial_fd < 0) {
		perror("Unable to open serial device: %s: %s \r\n", name, strerror(errno));
	}
	return serial_fd;
}

int serial_close(int fd)
{
}
