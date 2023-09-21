// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 SASANO Takayoshi <uaa@uaa.org.uk>

#include <stdexcept>
#include "yomi_mecab.h"

yomi_mecab::yomi_mecab(void)
{
	const char *mecab_argv[] = {"mecab", "-Oyomi"};

	mctx = mecab_new(sizeof(mecab_argv) / sizeof(char *),
			 (char **)mecab_argv);

	if (mctx == NULL)
		throw(std::invalid_argument("mecab_new error"));
}

int yomi_mecab::convert(char *out, const char *in)
{
	int ret = -1;
	const char *p;

	if ((p = mecab_sparse_tostr(mctx, in)) == NULL)
		goto fin;

	copy_result(out, p);
	
	ret = 0;
fin:
	return ret;
}

yomi_mecab::~yomi_mecab(void)
{
	mecab_destroy(mctx);
}
