// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 SASANO Takayoshi <uaa@uaa.org.uk>

#ifndef CONVERT_NGRAM_H
#define CONVERT_NGRAM_H

#include "convert.h"

class convert_ngram : public convert {
public:
	void do_file(FILE *fpi, FILE *fpo, int ngrams, int64_t limit);
	int64_t do_line(int ngrams, wchar_t *out_buf, wchar_t *in_buf);
};

#endif
