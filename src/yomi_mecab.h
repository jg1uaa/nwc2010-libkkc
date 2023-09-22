// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 SASANO Takayoshi <uaa@uaa.org.uk>

#ifndef YOMI_MECAB_H
#define YOMI_MECAB_H

#include <mecab.h>
#include "yomi.h"

class yomi_mecab : public yomi {
public:
	yomi_mecab(void);
	int convert(wchar_t *inout, size_t sz);
	~yomi_mecab(void);

private:
	mecab_t *mctx;  
};

#endif
