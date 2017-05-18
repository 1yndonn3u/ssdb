#ifndef SSDB_COMPARATOR_H_
#define SSDB_COMPARATOR_H_

#include "rocksdb/slice.h"
#include "rocksdb/comparator.h"

class SlotBytewiseComparatorImpl : public rocksdb::Comparator {
public:
	virtual const char *Name() const;

	virtual int Compare(const rocksdb::Slice &a, const rocksdb::Slice &b) const;

	virtual void FindShortestSeparator(std::string *start, const rocksdb::Slice &limit) const;

	virtual void FindShortSuccessor(std::string *key) const;

	static rocksdb::Comparator *getComparator();
private:
	SlotBytewiseComparatorImpl(){};
	static rocksdb::Comparator *ins;
};

#endif
