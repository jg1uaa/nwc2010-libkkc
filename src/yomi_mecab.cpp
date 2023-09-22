// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 SASANO Takayoshi <uaa@uaa.org.uk>

#include <stdexcept>
#include "yomi_mecab.h"

yomi_mecab::yomi_mecab(void)
{
	const char *mecab_argv[] = {"mecab", "-Oyomi"};

	if ((mctx = mecab_new(sizeof(mecab_argv) / sizeof(char *),
			      (char **)mecab_argv)) == NULL)
		throw(std::invalid_argument("mecab_new error"));
}

int yomi_mecab::convert(wchar_t *inout, size_t sz)
{
	int ret = -1;
	const char *p;
	char in[CONV_BUFSIZE_MBS], out[CONV_BUFSIZE_MBS];

	if ((ssize_t)wcstombs(in, inout, CONV_BUFSIZE_MBS) < 0 ||
	    (p = mecab_sparse_tostr(mctx, in)) == NULL)
		goto fin;

	copy_result(out, p);

	if ((ssize_t)mbstowcs(inout, out, sz) < 0)
		goto fin;

	ret = 0;
fin:
	return ret;
}

yomi_mecab::~yomi_mecab(void)
{
	mecab_destroy(mctx);
}
