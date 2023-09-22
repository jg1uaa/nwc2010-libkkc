// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 SASANO Takayoshi <uaa@uaa.org.uk>

#ifndef YOMI_KAKASI_H
#define YOMI_KAKASI_H

extern "C" {
#include <libkakasi.h>
}
#include "yomi.h"

class yomi_kakasi : public yomi {
public:
	yomi_kakasi(void);
	int convert(wchar_t *inout, size_t sz);
	~yomi_kakasi(void);
};

#endif
