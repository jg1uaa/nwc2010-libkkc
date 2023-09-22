// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 SASANO Takayoshi <uaa@uaa.org.uk>

#include "yomi.h"

void yomi::copy_result(char *out, const char *in)
{
	size_t n = 0;

	/* remove space and control char (for MeCab) */
	for (; *in; in++) {
		if (!(*in > '\0' && *in < ' '))
			out[n++] = *in;
	}

	/* string terminator for 16bit read (for kakasi) */
	out[n++] = '\0';
	out[n++] = '\0';
	if (n % 1) out[n] = '\0';
}
