#ifndef TYPED_STRATEGY_HPP
#define TYPED_STRATEGY_HPP

#include <cstring>
#include <new>

#include "strategy.hpp"
#include "is_value_type.hpp"

template<class T> struct TypedStrategy: Strategy {
	void* alloc(size_t n) { 
        T *t = new T[n]; 
        for (size_t i = 0; i < n; i++)
            operator new(sizeof(T), t + i);
        return t;
    }
	void free(void* ary) { delete[] static_cast<T*>(ary); }
	void move(void* dst, void* src, size_t n) {
        if (src == NULL) return;
		if (is_value_type<T>()) memcpy(dst, src, n*sizeof(T)); 
		else for (size_t i = 0; i < n; i++) static_cast<T*>(dst)[i] = static_cast<T*>(src)[i];
	}
};

#endif //TYPED_STRATEGY_HPP