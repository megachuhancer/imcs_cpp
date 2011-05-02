#ifndef MULTIPLY_STRATEGY_HPP
#define MULTIPLY_STRATEGY_HPP

#include "typed_strategy.hpp"

template<class T> struct MultiplyStrategy: TypedStrategy<T> {
	static const int MULTIPLIER = 2;
	static const int DIVIDER = 2;
	static const int TRESHOLD = 2; //+1

	size_t determine_size(size_t needed, size_t actual) {
		if (needed > actual) 
            return needed*MULTIPLIER;
		if (!needed) 
            return 0;
		if (actual/needed > TRESHOLD) 
            return actual/DIVIDER;
        return actual;
	}
};

#endif //MULTIPLY_STRATEGY_HPP