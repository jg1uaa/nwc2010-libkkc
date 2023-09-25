// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 SASANO Takayoshi <uaa@uaa.org.uk>

#include <stdexcept>
#include "yomi_chasen.h"

yomi_chasen::yomi_chasen(void)
{
	const char *chasen_argv[] = {"chasen", "-F", "%y", NULL};

	if (chasen_getopt_argv((char **)chasen_argv, NULL))
		throw(std::invalid_argument("chasen_getopt_argv error"));
}

int yomi_chasen::convert(wchar_t *inout, size_t sz)
{
	int ret = -1;
	const char *p;
	char temp[CONV_BUFSIZE_MBS];

	if ((ssize_t)wcstombs(temp, inout, CONV_BUFSIZE_MBS) < 0 ||
	    (p = chasen_sparse_tostr(temp)) == NULL)
		goto fin;

	copy_result(temp, p);

	if ((ssize_t)mbstowcs(inout, temp, sz) < 0)
		goto fin;

	ret = 0;
fin:
	return ret;
}

yomi_chasen::~yomi_chasen(void)
{
	/* do nothing */
}
