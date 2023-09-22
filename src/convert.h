// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 SASANO Takayoshi <uaa@uaa.org.uk>

#ifndef CONVERT_H
#define CONVERT_H

#include <cwchar>
#include <cstdio>
#include <cstdint>
#include "param.h"

#define BOS 1	// begin of sentence
#define EOS 2	// end of sentence

class convert {
public:
	virtual void do_file(FILE *fpi, FILE *fpo, int ngrams, int64_t limit) = 0;
	virtual int64_t do_line(int ngrams, wchar_t *out_buf, wchar_t *in_buf) = 0;

protected:
	int tokenize(wchar_t *tokens[], int max_token, wchar_t *in_buf);
	int check_token(wchar_t *token);
	int convert_yomi(wchar_t *yomi);
	int create_result(wchar_t *result, wchar_t *yomi, wchar_t *token);
  
	wchar_t in_line[CONV_BUFSIZE], out_line[CONV_BUFSIZE];
	wchar_t *token_wc[MAX_NGRAMS + 2];
	wchar_t yomi_wc[MAX_NGRAMS][CONV_BUFSIZE];
	wchar_t result[MAX_NGRAMS][CONV_BUFSIZE];
};

#endif
