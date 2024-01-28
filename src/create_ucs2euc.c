// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 SASANO Takayoshi <uaa@uaa.org.uk>

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <iconv.h>

#define SS2 0x8e
#define SS3 0x8f

#define TABLE_SIZE 65536

static uint16_t ucs2euc[TABLE_SIZE];
static uint16_t euc2ucs[TABLE_SIZE];

static int create_table(void)
{
	iconv_t cd;
	int ucs, euc;
	unsigned char in[2], out[4];
	char *in_p, *out_p;
	ssize_t n;
	size_t insize, outsize;

	memset(ucs2euc, 0, sizeof(ucs2euc));
	memset(euc2ucs, 0, sizeof(euc2ucs));

	if ((cd = iconv_open("EUC-JP", "UCS-2BE")) < 0) {
		printf("iconv_open\n");
		return -1;
	}

	for (ucs = 0; ucs < 0x10000; ucs++) {
		/* skip C0/C1 control, surrogate, private area */
		if ((ucs >= 0x0000 && ucs <= 0x001f) ||
		    (ucs >= 0x0080 && ucs <= 0x009f) ||
		    (ucs >= 0xd800 && ucs <= 0xdfff) ||
		    (ucs >= 0xe000 && ucs <= 0xf8ff))
			continue;

		in[0] = ucs >> 8;
		in[1] = ucs;

		in_p = (char *)in;
		out_p = (char *)out;
		insize = sizeof(in);
		outsize = sizeof(out);

		if ((n = iconv(cd, &in_p, &insize, &out_p, &outsize)) < 0)
			continue;

		switch (out[0]) {
		case 0x00 ... 0x7f:
		case SS2:
		case SS3:
			euc = 0;
			break;
		default:
			euc =(out[0] << 8) | out[1];
			break;
		}

		euc2ucs[euc] = ucs;
		ucs2euc[ucs] = euc;
	}

	iconv_close(cd);
	return 0;
}

static void dump_table(FILE *fp, uint16_t *table, int size)
{
	int i;

	for (i = 0; i < size; i++) {
		if (!(i % 8)) fprintf(fp, "\t");
		fprintf(fp, "0x%04x, ", table[i]);
		if (!((i + 1) % 8)) fprintf(fp, "\n");
	}
}

static int output_result(char *filename)
{
	FILE *fp;

	if ((fp = fopen(filename, "w")) == NULL)
		return -1;

	fprintf(fp, "// SPDX-License-Identifier: Unlicense\n\n");
	fprintf(fp, "#include <cstdint>\n\n");

	fprintf(fp, "uint16_t ucs2euc[%d] = {\n", TABLE_SIZE);
	dump_table(fp, ucs2euc, TABLE_SIZE);
	fprintf(fp, "};\n\n");

	fprintf(fp, "uint16_t euc2ucs[%d] = {\n", TABLE_SIZE);
	dump_table(fp, euc2ucs, TABLE_SIZE);
	fprintf(fp, "};\n\n");

	fclose(fp);

	return 0;
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("%s: usage [outfile]\n", argv[0]);
		goto fin0;
	}

	if (create_table())
		goto fin0;

	output_result(argv[1]);

fin0:
	return 0;
}
