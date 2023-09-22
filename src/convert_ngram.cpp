// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 SASANO Takayoshi <uaa@uaa.org.uk>

#include <cstring>
#include "convert_ngram.h"
#include "datastore.h"
#include "yomi.h"

int64_t convert_ngram::do_line(int ngrams, wchar_t *out_buf, wchar_t *in_buf)
{
	int64_t ret = -1;  
	int i, retry;
	wchar_t *wp, temp[CONV_BUFSIZE];
	long long c;

	if (tokenize(token_wc, ngrams + 1, in_buf) < 0)
		goto fin;
	c = wcstoll(token_wc[ngrams], NULL, 0);

	for (i = 0; i < ngrams; i++) {
		if (!wcscasecmp(token_wc[i], L"<S>")) {
			yomi_wc[i][0] = BOS;
			continue;
		} else if (!wcscasecmp(token_wc[i], L"</S>")) {
			yomi_wc[i][0] = EOS;
			continue;
		} else if (check_token(token_wc[i]) < 0) {
			goto fin;
		}

		yomi_wc[i][0] = L'\0';
		wcscpy(temp, token_wc[i]);
		do {
			if (yomi_engine->convert(temp, CONV_BUFSIZE) < 0)
				goto fin;

			/* sometimes yomi engine fails, retry */
			retry = convert_yomi(temp);
			if (retry < 0 ||
			    (retry > 0 && !wcscmp(yomi_wc[i], temp))) {
				/* error or nothing converted when retry */
				goto fin;
			}

			wcscpy(yomi_wc[i], temp);
		} while (retry);
	}

	for (i = 0; i < ngrams; i++) {
		if (create_result(result[i], yomi_wc[i], token_wc[i]) < 0)
			goto fin;
	}

	wp = out_buf;
	for (i = 0; i < ngrams; i++) {
		if (i) *wp++ = L' ';
		wcscpy(wp, result[i]);
		wp += wcslen(result[i]);
	}

	ret = c;
fin:
	return ret;
}

void convert_ngram::do_file(FILE *fpi, FILE *fpo, int ngrams, int64_t limit)
{
	data_store<int64_t> d;

	/* pass 1 */
	d.init();
	while(fgetws((in_line), CONV_BUFSIZE, fpi) != NULL) {
		int64_t c;
		if ((c = do_line(ngrams, out_line, in_line)) <= 0 ||
		    c < limit)
			continue;
		d.set(out_line, c);
	}

	/* pass 2 */
	for (auto i = d.db.begin(); i != d.db.end(); i++) {
		fprintf(fpo, "%ls\t%ld\n",
			d.db[i->first].key.c_str(), d.db[i->first].value);
	}
}
