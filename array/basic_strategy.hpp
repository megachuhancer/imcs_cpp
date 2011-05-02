#ifndef BASIC_STRATEGY_HPP
#define BASIC_STRATEGY_HPP

#include <cstring>

#include "typed_strategy.hpp"

template<class T> struct BasicStrategy: TypedStrategy<T> {
	size_t determine_size(size_t needed, size_t actual) { return needed; }
};

#endif //BASIC_STRATEGY_HPP