// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 SASANO Takayoshi <uaa@uaa.org.uk>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define TABLE_SIZE 65536

static uint16_t ucs2euc[TABLE_SIZE];
static uint16_t euc2ucs[TABLE_SIZE];

static int input_table(char *filename)
{
#define jis2euc(x) ((x) | 0x8080)

	int ret = -1;
	FILE *fp;
	char buf[256], *p;
	long sjis __attribute__((unused)), jis, unicode;

	if ((fp = fopen(filename, "r")) == NULL) {
		printf("file open error\n");
		goto fin0;
	}

	memset(ucs2euc, 0, sizeof(ucs2euc));
	memset(euc2ucs, 0, sizeof(euc2ucs));

	while (fgets(buf, sizeof(buf), fp) != NULL) {
		/* skip comment line */
		if (buf[0] == '#')
			continue;

		/* get character code numbers */
		sjis = strtol(buf, &p, 0);
		if (*p == '\0')
			continue;
		jis = strtol(p, &p, 0);
		if (*p == '\0')
			continue;
		unicode = strtol(p, &p, 0);

		/* skip invalid code */
		if (unicode <= 0 || unicode >= TABLE_SIZE ||
		    jis <= 0 || jis >= TABLE_SIZE)
			continue;

		/* create unicode <- -> EUC-JP table */
		ucs2euc[unicode] = jis2euc(jis);
		euc2ucs[jis2euc(jis)] = unicode;
	}

	ret = 0;

	fclose(fp);
fin0:
	return ret;
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

	fprintf(fp, "// SPDX-License-Identifier: Unicode-TOU\n\n");
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
	if (argc < 3) {
		printf("%s: usage [infile] [outfile]\n", argv[0]);
		goto fin0;
	}

	if (input_table(argv[1]))
		goto fin0;

	output_result(argv[2]);

fin0:
	return 0;
}
