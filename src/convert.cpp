// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 SASANO Takayoshi <uaa@uaa.org.uk>

#include <cstring>
#include <cstdlib>
#include "convert.h"

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

ssize_t convert::convert_token(char *mbs, wchar_t *wc, size_t sz)
{
	/* first character should not "ー" (U+30FC) */
	return (*wc == 0x30fc) ? -1 : (ssize_t)wcstombs(mbs, wc, sz);
}

int convert::convert_yomi(char *yomi, size_t sz)
{
	size_t i;
	int ret = -1;
	wchar_t temp[CONV_BUFSIZE];

	if ((ssize_t)mbstowcs(temp, yomi, CONV_BUFSIZE) < 0)
		goto fin;

	/* convert katakana -> hiragana */
	for (i = 0; i < wcslen(temp); i++) {
		/* CJK unified ideographs:
		 * retry convert (for kakasi errata) */
		if (temp[i] >= 0x4e00 && temp[i] <= 0x9fff) {
			ret = 1;
			goto fin;
		}

		/* result should be "ー", "ぁ" - "ん" , or "ァ" - "ヴ" */
		if (temp[i] != 0x30fc &&
		    !(temp[i] >= 0x3041 && temp[i] <= 0x3093) &&
		    !(temp[i] >= 0x30a1 && temp[i] <= 0x30f4))
			goto fin;

		/* convert to hiragana (except "ー" and "ヴ") */
		if (temp[i] >= 0x30a1 && temp[i] < 0x30f4)
			temp[i] -= 0x60;
	}

	if ((ssize_t)wcstombs(yomi, temp, sz) < 0)
		goto fin;

	ret = 0;
fin:
	return ret;
}

int convert::call_mecab(char *out, char *in)
{
	int ret = -1;
	const char *p;

	if ((p = mecab_sparse_tostr(mctx, in)) == NULL)
		goto fin;

	/* copy with removing garbage (trailing LF) */
	while (*p) {
		if (*p > '\0' && *p < ' ')
			break;
		if (*p == ' ')
			p++;
		else
			*out++ = *p++;
	}
	*out = '\0';

	ret = 0;
fin:
	return ret;
}

int convert::create_result(wchar_t *result, char *yomi, char *token, size_t sz)
{
	ssize_t n;
	int ret = -1;

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
		if ((n = (ssize_t)mbstowcs(result, yomi, sz)) < 0)
			goto fin;

		result[n++] = L'/';
		break;
	}

	if ((ssize_t)mbstowcs(result + n, token, sz - n) < 0)
		goto fin;

	ret = 0;
fin:
	return ret;
}
