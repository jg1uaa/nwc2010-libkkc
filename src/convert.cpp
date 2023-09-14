// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 SASANO Takayoshi <uaa@uaa.org.uk>

#include <cstring>
#include <endian.h>
#include "convert.h"

extern "C" {
#include <libkakasi.h>

extern uint16_t ucs2euc[65536];
extern uint16_t euc2ucs[65536];
};

int convert::tokenize(wchar_t *tokens[], int max_token, wchar_t *in)
{
	int i;
	wchar_t *wp;

	for (i = 0; i < max_token; i++) {
		tokens[i] = wcstok(i ? NULL : in, L" \t\n", &wp);
		if (tokens[i] == NULL)
			return -1;		  
	}

	return 0;
}

int convert::convert_token(uint16_t *euc, wchar_t *wc)
{
	int i, ret = -1;

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

int convert::revert_token(wchar_t *wc, uint16_t *euc)
{
	int i, ret = -1;

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

void convert::terminate_fix(char *str)
{
	size_t n;

	n = strlen(str) + 1;

	/* fix string terminator for 16bit read */
	str[n++] = '\0';
	if (n % 1) str[n] = '\0';
}

int convert::check_yomi(uint16_t *euc)
{
	/* kakasi returns nothing when convert failed? */
	if (!strlen((char *)euc))
		return -1;

	/* if first character is "ー" (JIS 0x213c), filter out
	 * (this should be done before calling kakasi XXX) */
	if (be16toh(*euc) == 0xa1bc)
		return -1;

	/* kakasi sometimes returns invalid yomi character (kanji) */
	for(; *euc; euc++) {
		if (be16toh(*euc) >= 0xb0a1) /* JIS 0x3021 */
			return 1;
	}

	return 0;
}

int convert::call_kakasi(char *out, char *in)
{
	int ret;
	char *p;
	char temp[CONV_BUFSIZE * sizeof(uint16_t)] __attribute__((aligned(4)));

	strcpy(temp, in);

	/* kakasi sometimes fails kanji -> hiragana conversion,
	 * for example: "蹴っ飛ばす" -> "けっ飛ばす" not "けっとばす".
	 * retry is a workaround, max retry count is preliminary XXX */
	for (size_t i = 0; i < strlen(in); i++) {
		p = kakasi_do(temp);
		strcpy(out, p);
		terminate_fix(out);
		kakasi_free(p);

		ret = check_yomi((uint16_t *)out);
		if (ret < 1)
			break;

		strcpy(temp, p);
	}

	return ret;
}

int convert::create_result(wchar_t *result, uint16_t *yomi, uint16_t *token)
{
	int n, ret = -1;

	switch (yomi[0]) {
	case BOS:
		wcscpy(result, L"<S>");
		ret = BOS;
		goto fin;
	case EOS:
		wcscpy(result, L"</S>");
		ret = EOS;
		goto fin;
	case 0:
		n = 0;
		break;
	default:
		if ((n = revert_token(result, yomi)) < 0)
			goto fin;

		result[n++] = L'/';
		break;
	}

	if (revert_token(result + n, token) < 0)
		goto fin;

	ret = 0;
fin:
	return ret;
}
