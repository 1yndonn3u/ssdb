/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/

#ifndef SSDB_TRANSACTION_H_
#define SSDB_TRANSACTION_H_

#include "rocksdb/db.h"
#include "rocksdb/write_batch.h"
#include "../util/bytes.h"

class SSDB;

class Transaction {
public:
	typedef rocksdb::Status Status;

private:
	typedef rocksdb::Slice Slice;
	typedef rocksdb::WriteBatch WriteBatch;
	typedef rocksdb::WriteOptions WriteOptions;

	SSDB *db;
	WriteBatch updates;
	std::string lock_key;

public:
	Transaction(SSDB *db_, const std::string &key);
	Transaction(SSDB *db_, const Bytes &key);
	~Transaction();

public:
	void begin();
	void rollback();
	Status commit();

	void del(const Bytes &key);
	void put(const Bytes &key, const Bytes &val);
	void del(const std::string &key);
	void put(const std::string &key, const std::string &val);
};

#endif
