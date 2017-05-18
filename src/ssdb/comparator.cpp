#include <stdint.h>
#include "comparator.h"

const char *SlotBytewiseComparatorImpl::Name() const {
	return "ssdb.SlotBytewiseComparatorImpl";
}

int SlotBytewiseComparatorImpl::Compare(const rocksdb::Slice &a, const rocksdb::Slice &b) const {
	int16_t slota = *reinterpret_cast<const int16_t*>(a.data()+a.size()-sizeof(int16_t));
	int16_t slotb = *reinterpret_cast<const int16_t*>(b.data()+b.size()-sizeof(int16_t));
	if(slota < slotb) {
		return -1;
	} else if(slota > slotb) {
		return 1;
	}
	rocksdb::Slice aa = rocksdb::Slice(a.data(), a.size()-sizeof(int16_t));
	rocksdb::Slice bb = rocksdb::Slice(b.data(), b.size()-sizeof(int16_t));
	return rocksdb::BytewiseComparator()->Compare(aa, bb);
}

void SlotBytewiseComparatorImpl::FindShortestSeparator(std::string *start, const rocksdb::Slice &limit) const {
	//return rocksdb::BytewiseComparator()->FindShortestSeparator(start, limit);
}

void SlotBytewiseComparatorImpl::FindShortSuccessor(std::string *key) const {
	//return rocksdb::BytewiseComparator()->FindShortSuccessor(key);
}

rocksdb::Comparator *SlotBytewiseComparatorImpl::getComparator() {
	if (ins == NULL) {
		ins = new SlotBytewiseComparatorImpl();
	}
	return ins;
}

rocksdb::Comparator *SlotBytewiseComparatorImpl::ins = NULL;
