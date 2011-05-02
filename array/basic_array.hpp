#ifndef BASIC_ARRAY_HPP
#define BASIC_ARRAY_HPP

#include <cstddef>

#include "strategy.hpp"

class BasicArray {
protected:
    void *ary;
    size_t sz, actual_sz;
	Strategy *strategy;
	void destroy();
	void chk_capacity(size_t needed);
public:
	class OutOfRange {};
	class BasicIterator {
		void shift(int v);
	protected:
        BasicArray *ary;
		size_t pos;

		BasicIterator(BasicArray* a, size_t p): ary(a), pos(p) {}
	public:
		class IteratorIsNotDereferencable {};
		bool is_dereferencable() const { return pos < ary->sz; };

		BasicIterator(BasicIterator& i): ary(i.ary), pos(i.pos) {};

        bool operator==(const BasicIterator& rhs) const { return ary == rhs.ary && pos == rhs.pos; }
        bool operator!=(const BasicIterator& rhs) const { return !(*this == rhs); }

		BasicIterator& operator++();
		BasicIterator& operator--();
		BasicIterator& operator+=(int rhs);
		BasicIterator& operator-=(int rhs);
		BasicIterator operator++(int);
		BasicIterator operator--(int);
		BasicIterator operator+(int rhs);
		BasicIterator operator-(int rhs);
	};

	BasicArray(): ary(NULL), sz(0), actual_sz(0), strategy(NULL) {}
	~BasicArray();

	size_t size() const { return sz; } 
	void resize(size_t n);
	size_t capacity() const { return actual_sz; }
	bool empty() const { return sz == 0; }
	void reserve(size_t n) { chk_capacity(n > actual_sz ? n : actual_sz); }
    void clear() { sz = 0; chk_capacity(sz); }
};

#endif //BASIC_ARRAY_HPP