// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 SASANO Takayoshi <uaa@uaa.org.uk>

#include <stdexcept>
#include <cstring>
#include <cstdint>
#include <endian.h>
#include "yomi_kakasi.h"

extern uint16_t ucs2euc[65536];
extern uint16_t euc2ucs[65536];

/* kakasi needs to work with EUC-JP, not UTF-8 */
static ssize_t convert_euc(uint16_t *euc, wchar_t *wc)
{
	ssize_t i, ret = -1;

	for (i = 0; *wc; i++, wc++) {
		if (*wc >= (wchar_t)(sizeof(ucs2euc) / sizeof(uint16_t)))
			goto fin;

		/* use bigendian to put result into kakasi directly */
		if (!(euc[i] = htobe16(ucs2euc[*wc])))
			goto fin;
	}
	euc[i] = 0;

	ret = i;
fin:
	return ret;
}

static ssize_t revert_euc(wchar_t *wc, uint16_t *euc)
{
	ssize_t i, ret = -1;

	for (i = 0; *euc; i++, euc++) {
		/* euc is stored as bigendian */
		if (!(wc[i] = euc2ucs[be16toh(*euc)]))
			goto fin;
	}
	wc[i] = L'\0';

	ret = i;
fin:
	return ret;
}

yomi_kakasi::yomi_kakasi(void)
{
	const char *kakasi_argv[] = {"kakasi", "-ieuc", "-KH", "-JH"};

	if (kakasi_getopt_argv(sizeof(kakasi_argv) / sizeof(char *),
			       (char **)kakasi_argv))
		throw(std::invalid_argument("kakasi_new error"));
}

int yomi_kakasi::convert(wchar_t *inout, size_t sz)
{
	int ret = -1;
	char *p;
	char temp[CONV_BUFSIZE_MBS] __attribute__((aligned(4)));

	if (convert_euc((uint16_t *)temp, inout) < 0 ||
	    (p = kakasi_do(temp)) == NULL)
		goto fin;

	copy_result(temp, p);
	kakasi_free(p);

	if (revert_euc(inout, (uint16_t *)temp) < 0)
		goto fin;

	ret = 0;
fin:
	return ret;
}

yomi_kakasi::~yomi_kakasi(void)
{
	/* do nothing */
}
