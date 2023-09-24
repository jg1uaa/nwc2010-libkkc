// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 SASANO Takayoshi <uaa@uaa.org.uk>

#include <cstring>
#include "convert_ngram.h"
#include "datastore.h"
#include "yomi.h"

int64_t convert_ngram::do_line(int ngrams, wchar_t *out_buf, wchar_t *in_buf)
{
	int64_t ret = -1;  
	int i;
	wchar_t *wp;
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

		/* try katakana/hiragana -> hiragana convert */
		wcscpy(yomi_wc[i], token_wc[i]);
		if (!convert_yomi(yomi_wc[i]))
			continue;

		/* retry with yomi engine */
		wcscpy(yomi_wc[i], token_wc[i]);
		if (yomi_engine->convert(yomi_wc[i], CONV_BUFSIZE) < 0)
			goto fin;

		/* sometimes yomi engine fails, discard */
		if (convert_yomi(yomi_wc[i]) < 0)
			goto fin;
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
