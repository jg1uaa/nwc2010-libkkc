// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 SASANO Takayoshi <uaa@uaa.org.uk>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <clocale>
#include "convert_ngram.h"

int main(int argc, char *argv[])
{
	FILE *fpi, *fpo;
	int ngrams;
	int64_t limit;
	convert_ngram cn;
	
	if (argc < 4) {
		fprintf(stderr, "%s: usage [ngrams] [infile] [outfile] "
			"[limit]\n", argv[0]);
		goto fin0;
	}

	ngrams = atoi(argv[1]);
	if (ngrams < 1 || ngrams > 3) {
		fprintf(stderr, "invalid ngram value\n");
		goto fin0;
	}

	if ((fpi = fopen(argv[2], "r")) == NULL) {
		fprintf(stderr, "file open error (in)\n");
		goto fin0;
	}

	if ((fpo = fopen(argv[3], "w")) == NULL) {
		fprintf(stderr, "file open error (out)\n");
		goto fin1;
	}

	limit = (argc < 5) ? 0 : atoll(argv[4]);

	setlocale(LC_ALL, "ja_JP.UTF-8");

	cn.do_file(fpi, fpo, ngrams, limit);
	
	fclose(fpo);
fin1:
	fclose(fpi);
fin0:
	return 0;
}
