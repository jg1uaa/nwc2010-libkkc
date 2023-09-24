// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 SASANO Takayoshi <uaa@uaa.org.uk>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <clocale>
#include <unistd.h>
#include "convert_ngram.h"
#include "yomi_kakasi.h"
#include "yomi_mecab.h"
#include "yomi_juman.h"

yomi *yomi_engine;

#define ENGINE_KAKASI	0
#define ENGINE_MECAB	1
#define ENGINE_JUMAN	2

int main(int argc, char *argv[])
{
	FILE *fpi, *fpo;
	char *infile = NULL, *outfile = NULL;
	int ch, ngrams = 0, engine = ENGINE_KAKASI;
	int64_t limit = 0;
	convert_ngram cn;

	while ((ch = getopt(argc, argv, "i:o:n:l:KMJ")) != -1) {
		switch (ch) {
		case 'i':
			infile = optarg;
			break;
		case 'o':
			outfile = optarg;
			break;
		case 'n':
			ngrams = atoi(optarg);
			break;
		case 'l':
			limit = atoll(optarg);
			break;
		case 'K':
			engine = ENGINE_KAKASI;
			break;
		case 'M':
			engine = ENGINE_MECAB;
			break;
		case 'J':
			engine = ENGINE_JUMAN;
			break;
		default:
			break;
		}
	}

	if (infile == NULL || outfile == NULL || ngrams < 1 || ngrams > 3) {
		fprintf(stderr, "usage: %s -i [infile] -o [outfile] "
			"-n [ngrams(1-3)]\n", argv[0]);
		goto fin0;
	}

	if ((fpi = fopen(infile, "r")) == NULL) {
		fprintf(stderr, "file open error (in)\n");
		goto fin0;
	}

	if ((fpo = fopen(outfile, "w")) == NULL) {
		fprintf(stderr, "file open error (out)\n");
		goto fin1;
	}

	setlocale(LC_ALL, "ja_JP.UTF-8");

	switch (engine) {
	case ENGINE_MECAB:
		yomi_engine = new yomi_mecab;
		break;
	case ENGINE_JUMAN:
		yomi_engine = new yomi_juman;
		break;
	default:
		yomi_engine = new yomi_kakasi;
		break;
	}

	cn.do_file(fpi, fpo, ngrams, limit);

	delete yomi_engine;

	fclose(fpo);
fin1:
	fclose(fpi);
fin0:
	return 0;
}
