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

class convert {
public:
	virtual void do_file(FILE *fpi, FILE *fpo, int ngrams, int64_t limit) = 0;
	virtual int64_t do_line(int ngrams, wchar_t *out_buf, wchar_t *in_buf) = 0;

protected:
	int tokenize(wchar_t *tokens[], int max_token, wchar_t *in_buf);
	int convert_token(uint16_t *euc, wchar_t *wc);
	int revert_token(wchar_t *wc, uint16_t *euc);
	void terminate_fix(char *str);
	int check_yomi(uint16_t *euc);
	int call_kakasi(char *out, char *in);
	int create_result(wchar_t *result, uint16_t *yomi, uint16_t *token);
  
	wchar_t in_line[CONV_BUFSIZE], out_line[CONV_BUFSIZE];
	wchar_t *token_wc[MAX_NGRAMS + 2];
	uint16_t token_euc[MAX_NGRAMS][CONV_BUFSIZE];
	uint16_t yomi_euc[MAX_NGRAMS][CONV_BUFSIZE];
	wchar_t result[MAX_NGRAMS][CONV_BUFSIZE];
};

#endif
