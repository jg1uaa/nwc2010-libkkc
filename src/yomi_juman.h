// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 SASANO Takayoshi <uaa@uaa.org.uk>

#ifndef YOMI_JUMAN_H
#define YOMI_JUMAN_H

#include "yomi.h"
#include "juman_client.h"

class yomi_juman : public yomi {
public:
	yomi_juman(void);
	int convert(wchar_t *inout, size_t sz);
	~yomi_juman(void);

private:
	juman_client *client;  
};

#endif
