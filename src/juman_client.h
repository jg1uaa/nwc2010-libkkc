// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 SASANO Takayoshi <uaa@uaa.org.uk>

#ifndef JUMAN_CLIENT_H
#define JUMAN_CLIENT_H

#define CLIENT_BUFFER_SIZE 2048

class juman_client {
public:
	int connect(void);
	int disconnect(void);
	ssize_t send(const char *str);
	ssize_t recv(void);
	int check_response(unsigned short code);
	ssize_t query(char *inout, ssize_t size);

private:
	char buffer[CLIENT_BUFFER_SIZE];
	int fd;
};

#endif
