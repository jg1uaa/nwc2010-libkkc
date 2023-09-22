// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 SASANO Takayoshi <uaa@uaa.org.uk>

#ifndef YOMI_H
#define YOMI_H

#include <cwchar>
#include "param.h"

class yomi {
public:
	virtual int convert(wchar_t *inout, size_t sz) = 0;
	virtual ~yomi(){};  

protected:
	void copy_result(char *out, const char *in);
};

extern yomi *yomi_engine;

#endif
