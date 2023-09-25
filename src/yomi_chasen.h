// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 SASANO Takayoshi <uaa@uaa.org.uk>

#ifndef YOMI_CHASEN_H
#define YOMI_CHASEN_H

#include <chasen.h>
#include "yomi.h"

class yomi_chasen : public yomi {
public:
	yomi_chasen(void);
	int convert(wchar_t *inout, size_t sz);
	~yomi_chasen(void);
};

#endif
