#include "basic_array.hpp"

void BasicArray::BasicIterator::shift(int v) {
	size_t t = pos + v;
	if (t > ary->sz) throw OutOfRange();
	pos = t;
}

BasicArray::BasicIterator& BasicArray::BasicIterator::operator++() { 
	shift(1);
	return *this; 
}

BasicArray::BasicIterator& BasicArray::BasicIterator::operator--() { 
	shift(-1);
	return *this; 
}

BasicArray::BasicIterator BasicArray::BasicIterator::operator++(int) { 
	BasicIterator t(*this); 
	shift(1);
	return t; 
}

BasicArray::BasicIterator BasicArray::BasicIterator::operator--(int) { 
	BasicIterator t(*this); 
	shift(-1);
	return t; 
}

BasicArray::BasicIterator& BasicArray::BasicIterator::operator+=(int rhs) { 
	shift(rhs);
	return *this; 
}

BasicArray::BasicIterator& BasicArray::BasicIterator::operator-=(int rhs) { 
	shift(-rhs);
	return *this; 
}

BasicArray::BasicIterator BasicArray::BasicIterator::operator+(int rhs) { 
	BasicIterator t(*this);
	t += rhs;
	return t;
}

BasicArray::BasicIterator BasicArray::BasicIterator::operator-(int rhs) { 
	BasicIterator t(*this);
	t -= rhs;
	return t;
}