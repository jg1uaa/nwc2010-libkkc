// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 SASANO Takayoshi <uaa@uaa.org.uk>

#include <stdexcept>
#include "yomi_juman.h"

yomi_juman::yomi_juman(void)
{
	client = new juman_client;

	if (client->connect() < 0)
		throw(std::invalid_argument("juman connect error"));
}

int yomi_juman::convert(wchar_t *inout, size_t sz)
{
	int ret = -1;
	char temp[CONV_BUFSIZE_MBS];

	if ((ssize_t)wcstombs(temp, inout, CONV_BUFSIZE_MBS) < 0 ||
	    client->query(temp, sizeof(temp)) < 0)
		goto fin;

	/* no need to copy_result(), use temp[] directly */

	if ((ssize_t)mbstowcs(inout, temp, sz) < 0)
		goto fin;

	ret = 0;
fin:
	return ret;
}

yomi_juman::~yomi_juman(void)
{
	client->disconnect();
	delete client;
}
