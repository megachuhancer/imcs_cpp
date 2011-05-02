#ifndef TYPED_ARRAY_HPP
#define TYPED_ARRAY_HPP

#include "basic_array.hpp"
#include "basic_strategy.hpp"
#include "multiply_strategy.hpp"

template<class T, class Strategy = BasicStrategy<T> > class Array: public BasicArray {
	void init(const Array& x);
    T *ary() const { return static_cast<T*>(BasicArray::ary); }
public:
    class ContainerIsEmpty {};

	class Iterator: public BasicIterator {
		friend class Array;
		Iterator(BasicArray* a, size_t p): BasicIterator(a, p) {}
	public:
		Iterator(BasicIterator& i): BasicIterator(i) {}

		Iterator& operator++() { return static_cast<Iterator&>(BasicIterator::operator++()); }
		Iterator& operator--() { return static_cast<Iterator&>(BasicIterator::operator--()); }
		Iterator& operator+=(int rhs) { return static_cast<Iterator&>(BasicIterator::operator+=(rhs)); }
		Iterator& operator-=(int rhs) { return static_cast<Iterator&>(BasicIterator::operator-=(rhs)); }
		Iterator operator++(int) { return BasicIterator::operator++(0); }
		Iterator operator--(int) { return BasicIterator::operator--(0); }
		Iterator operator+(int rhs) { return BasicIterator::operator+(rhs); }
		Iterator operator-(int rhs) { return BasicIterator::operator-(rhs); }

		T& operator*() const {
			if (!is_dereferencable()) throw IteratorIsNotDereferencable();
            return (static_cast<Array<T>*>(ary)->ary())[pos];
		}
	};

    Array() { BasicArray::strategy = new Strategy(); }
    Array(size_t n, const T& x = T());
	Array(Iterator first, Iterator last);
	Array(const Array& x);
	Array& operator=(const Array& x);

	Iterator begin() const { return Iterator(const_cast<Array*>(this), 0); }
	Iterator end() const { return Iterator(const_cast<Array*>(this), sz); }
	
	T& operator[](size_t pos) const { return ary()[pos]; }
	T& at(size_t pos) const { if (pos >= sz) throw OutOfRange(); return (*this)[pos]; }
	T& front() const { return (*this)[0]; }
	T& back() const { return (*this)[sz - 1]; }

	void push_back(const T& x) { insert(end(), x); }
	void pop_back() { erase(end()); };
	Iterator insert(Iterator pos, const T& x);
	Iterator erase(Iterator pos);
};

template<class T, class Strategy> Array<T, Strategy>::Array(size_t n, const T& x = T()) {
    BasicArray::strategy = new Strategy();
    reserve(n); 
    BasicArray::sz = n;
    for (size_t i = 0; i < n; i++) ary()[i] = x;
}

template<class T, class Strategy> Array<T, Strategy>::Array(Iterator first, Iterator last) { 
    BasicArray::strategy = new Strategy();
    for (; first != last; ++first) 
        push_back(*first); 
}

template<class T, class Strategy> Array<T, Strategy>::Array(const Array& x) {
    BasicArray::strategy = new Strategy(); 
    init(x); 
}

template<class T, class Strategy> Array<T, Strategy>& Array<T, Strategy>::operator=(const Array& x) {
    destroy(); 
    init(x); 
    return *this;
}

template<class T, class Strategy> void Array<T, Strategy>::init(const Array& x) {
	reserve(x.sz); 
	sz = x.sz;
	for(size_t i = 0; i < x.sz; i++) ary()[i] = x.ary()[i];
}

template<class T, class Strategy> typename Array<T, Strategy>::Iterator Array<T, Strategy>::insert(Iterator pos, const T& x) {
    reserve(sz + 1);
    sz++;
    for (size_t i = sz - 1; i > pos.pos; i--) 
        ary()[i] = ary()[i - 1];
    ary()[pos.pos] = x;
    return pos;
}

template<class T, class Strategy> typename Array<T, Strategy>::Iterator Array<T, Strategy>::erase(Iterator pos) {
    if (!sz) throw ContainerIsEmpty();
    sz--;
    for (size_t i = pos.pos; i < sz; i++)
        ary()[i] = ary()[i + 1];
    chk_capacity(sz);
    return pos;
}

#endif //TYPED_ARRAY_HPP