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

int convert::convert_yomi(wchar_t *yomi)
{
	size_t i, len = wcslen(yomi);
	int ret = -1;

	if (!len)
		goto fin;

	/* convert katakana -> hiragana */
	for (i = 0; i < wcslen(yomi); i++) {
		if (yomi[i] != L'ー' &&
		    !(yomi[i] >= L'ぁ' && yomi[i] <= L'ん') &&
		    !(yomi[i] >= L'ァ' && yomi[i] <= L'ヴ'))
			goto fin;

		/* convert to hiragana (except "ー" and "") */
		if (yomi[i] >= L'ァ' && yomi[i] < L'ヴ')
			yomi[i] -= 0x60;
	}

	ret = 0;
fin:
	return ret;
}

int convert::check_token(wchar_t *token)
{
	size_t i;
	int ret = -1;

	/* check first character */
	if (*token == L'ー' || *token == L'々')
		goto fin;

	/* some ideographic marks and hiragana, katakana, CJK is permitted */
	for (i = 0; i < wcslen(token); i++) {
		if (token[i] != L'ー' && token[i] != L'々' &&
		    !(token[i] >= L'ぁ' && token[i] <= L'ん') &&
		    !(token[i] >= L'ァ' && token[i] <= L'ヶ') &&
		    !(token[i] >= 0x4e00 && token[i] <= 0x9fff))
			goto fin;
	}

	ret = 0;
fin:
	return ret;
}

int convert::create_result(wchar_t *result, wchar_t *yomi, wchar_t *token)
{
	size_t n;
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
		wcscpy(result, yomi);
		n = wcslen(result);
		result[n++] = L'/';
		break;
	}

	wcscpy(result + n, token);
	ret = 0;
fin:
	return ret;
}
