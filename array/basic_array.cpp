#include "basic_array.hpp"

void BasicArray::destroy() { 
	strategy->free(ary); 
	sz = 0; 
} 

void BasicArray::chk_capacity(size_t needed) {
	size_t new_sz = strategy->determine_size(needed, actual_sz);
	if (new_sz != actual_sz) {
		void *t = strategy->alloc(new_sz);
        strategy->move(t, ary, needed < sz ? needed : sz);
		strategy->free(ary);
		ary = t;
        actual_sz = new_sz;
	}
}

BasicArray::~BasicArray() { 
	destroy(); 
	delete strategy; 
}

void BasicArray::resize(size_t n) { 
	chk_capacity(n); 
    sz = n;
}