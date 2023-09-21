// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 SASANO Takayoshi <uaa@uaa.org.uk>

#ifndef CONVERT_H
#define CONVERT_H

#include <cwchar>
#include <cstdio>
#include <cstdint>

#define BOS 1	// begin of sentence
#define EOS 2	// end of sentence

#define MAX_NGRAMS 3
#define CONV_BUFSIZE 512
#define CONV_BUFSIZE_MBS (CONV_BUFSIZE * sizeof(wchar_t))

class convert {
public:
	virtual void do_file(FILE *fpi, FILE *fpo, int ngrams, int64_t limit) = 0;
	virtual int64_t do_line(int ngrams, wchar_t *out_buf, wchar_t *in_buf) = 0;

protected:
	int tokenize(wchar_t *tokens[], int max_token, wchar_t *in_buf);
	ssize_t convert_token(char *mbs, wchar_t *wc, size_t sz);
	int convert_yomi(char *yomi, size_t sz);
	int create_result(wchar_t *result, char *yomi, char *token, size_t sz);
  
	wchar_t in_line[CONV_BUFSIZE], out_line[CONV_BUFSIZE];
	wchar_t *token_wc[MAX_NGRAMS + 2];
	char token_mbs[MAX_NGRAMS][CONV_BUFSIZE_MBS];
	char yomi_mbs[MAX_NGRAMS][CONV_BUFSIZE_MBS];
	wchar_t result[MAX_NGRAMS][CONV_BUFSIZE];
};

#endif
