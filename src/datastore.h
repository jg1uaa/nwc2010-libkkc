// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 SASANO Takayoshi <uaa@uaa.org.uk>

#ifndef DATASTORE_H
#define DATASTORE_H

#include <string>
#include <map>

template <typename T> struct data_entry {
	std::wstring key;
	T value;
};

template <typename T> class data_store {
public:
	std::map<std::wstring, data_entry<T>> db;

	void set(wchar_t *key, T value) {
		data_entry<T> d;

		d.key = key;
		d.value = value;

		db.emplace(d.key, d);
	};

	void init(void) {
		db.clear();
	};
};

#endif
