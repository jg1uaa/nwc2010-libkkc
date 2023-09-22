// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 SASANO Takayoshi <uaa@uaa.org.uk>

#include <stdexcept>
#include <cstring>
#include "yomi_kakasi.h"

yomi_kakasi::yomi_kakasi(void)
{
	const char *kakasi_argv[] = {"kakasi", "-iutf8", "-KH", "-JH"};

	if (kakasi_getopt_argv(sizeof(kakasi_argv) / sizeof(char *),
			       (char **)kakasi_argv))
		throw(std::invalid_argument("kakasi_new error"));
}

int yomi_kakasi::convert(wchar_t *inout, size_t sz)
{
	int ret = -1;
	char *p;
	char in[CONV_BUFSIZE_MBS], out[CONV_BUFSIZE_MBS];

	if ((ssize_t)wcstombs(in, inout, CONV_BUFSIZE_MBS) < 0 ||
	    (p = kakasi_do((char *)in)) == NULL)
		goto fin;

	copy_result(out, p);
	kakasi_free(p);

	if ((ssize_t)mbstowcs(inout, out, sz) < 0)
		goto fin;

	ret = 0;
fin:
	return ret;
}

yomi_kakasi::~yomi_kakasi(void)
{
	/* do nothing */
}
