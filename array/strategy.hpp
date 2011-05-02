#ifndef STRATEGY_HPP
#define STRATEGY_HPP

#include <cstddef>

struct Strategy {
	virtual ~Strategy() {}
	virtual void* alloc(size_t n) = 0;
	virtual void free(void* ary) = 0;
	virtual void move(void* dst, void* src, size_t n) = 0;
	virtual size_t determine_size(size_t needed, size_t actual) = 0;
};

#endif //STRATEGY_HPP