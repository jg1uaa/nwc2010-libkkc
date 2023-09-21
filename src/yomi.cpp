// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 SASANO Takayoshi <uaa@uaa.org.uk>

#include "yomi.h"

void yomi::copy_result(char *out, const char *in)
{
	while (*in) {
		if (*in > '\0' && *in < ' ')
			break;
		if (*in == ' ')
			in++;
		else
			*out++ = *in++;
	}
	*out = '\0';
}
