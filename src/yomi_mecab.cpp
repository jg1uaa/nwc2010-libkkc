// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 SASANO Takayoshi <uaa@uaa.org.uk>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include "yomi_mecab.h"

yomi_mecab::yomi_mecab(void)
{
	char *p, arg[384] = "mecab -Oyomi";

	if ((p = getenv("MECAB_DIC")) != NULL && strlen(p)) {
		size_t n = strlen(arg);
		snprintf(arg + n, sizeof(arg) - n, " -d%s", p);
		fprintf(stderr, "dictionary at %s\n", p);
	}

	if ((mctx = mecab_new2(arg)) == NULL)
		throw(std::invalid_argument("mecab_new2 error"));
}

int yomi_mecab::convert(wchar_t *inout, size_t sz)
{
	int ret = -1;
	const char *p;
	char temp[CONV_BUFSIZE_MBS];

	if ((ssize_t)wcstombs(temp, inout, CONV_BUFSIZE_MBS) < 0 ||
	    (p = mecab_sparse_tostr(mctx, temp)) == NULL)
		goto fin;

	copy_result(temp, p);

	if ((ssize_t)mbstowcs(inout, temp, sz) < 0)
		goto fin;

	ret = 0;
fin:
	return ret;
}

yomi_mecab::~yomi_mecab(void)
{
	mecab_destroy(mctx);
}
