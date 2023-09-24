// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 SASANO Takayoshi <uaa@uaa.org.uk>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "juman_client.h"

static struct addrinfo *acquire_address_info(void)
{
	struct addrinfo hints, *res;
	char *server, *port; 
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; /* JUMAN has no IPv6 support */
	hints.ai_flags = AI_NUMERICSERV;
	hints.ai_socktype = SOCK_STREAM;

	server = getenv("JUMANSERVER");
	port = getenv("JUMANPORT");

	return getaddrinfo((server == NULL) ? "127.0.0.1" : server,
			   (port == NULL) ? "32000" : port, &hints, &res) ?
		NULL : res;
}

static int open_tcp_client(void)
{
	int s = -1;
	struct addrinfo *res, *res0;

	if ((res0 = acquire_address_info()) == NULL)
		goto fin0;

	for (res = res0; res; res = res->ai_next) {
		if ((s = socket(res->ai_family, res->ai_socktype,
				res->ai_protocol)) < 0)
			continue;

		if (connect(s, res->ai_addr, res->ai_addrlen) >= 0)
			break;

		close(s);
		s = -1;
	}

	freeaddrinfo(res0);
fin0:
	return s;
}

int juman_client::connect(void)
{
	int ret = -1;
	char run[] = "RUN -b -c";

	if ((fd = open_tcp_client()) < 0 || check_response(200))
		goto fin;

	if (send(run) < 0 || check_response(200))
		goto fin;

	ret = 0;
fin:
	return ret;
}

int juman_client::disconnect(void)
{
	const char escape[] = "\x0b";
	const char quit[] = "QUIT";

	send(escape);
	recv();
	send(quit);
	check_response(205);

	return close(fd);
}

ssize_t juman_client::send(const char *str)
{
	ssize_t n = strlen(str);
	const char cr = '\n';

	return (write (fd, str, n) < n || write (fd, &cr, 1) < 1) ?
		-1 : (n + 1);
}

ssize_t juman_client::recv(void)
{
	ssize_t n;

	for (n = 0; n < CLIENT_BUFFER_SIZE - 1; n++) {
		if (read (fd, &buffer[n], 1) < 1)
			return -1;

		if (buffer[n] == '\n')
			break;
	}
	buffer[n] = '\0';

	return n;
}

int juman_client::check_response(unsigned short code)
{
	char tmp[16];

	snprintf(tmp, sizeof(tmp), "%d ", code);
	return (recv() < 0 || strstr(buffer, tmp) != buffer) ? -1 : 0;
}

static char *get_yomi_field(char *in, ssize_t *length)
{
	char *p, *q, *result = NULL;

	/* yomi field is located at second place */
	if ((p = strchr(in, ' ')) == NULL)
		goto fin;

	if ((q = strchr(++p, ' ')) != NULL)
		*q = '\0';
	
	*length = strlen(p);
	result = p;
fin:
	return result;
}

ssize_t juman_client::query(char *inout, ssize_t size)
{
	ssize_t line, len, pos, ret = -1;
	char *p;
	bool copy = true;

	if (send(inout) < 0)
		goto fin;

	if ((line = mbstowcs(NULL, inout, 0)) < 1)
		goto fin;

	/* make room for terminate character */
	size--;

	/* max response lines: numbers of character + EOS line */
	pos = 0;
	for (; line >= 0; line--) {
		if (recv() < 0)
			goto fin;

		if (!strcmp(buffer, "EOS"))
			break;

		if (!copy || (p = get_yomi_field(buffer, &len)) == NULL ||
		    len < 1 || (size - pos) < len) {
			copy = false;
			continue;
		}

		strcpy(inout + pos, p);
		pos += len;
	}
	ret = pos;
fin:
	return ret;
} 
