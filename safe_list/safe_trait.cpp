#include "list.hpp"

void SafeTrait::add_iterator(BasicList::BasicIterator* iter) {
	iters.push_back(iter);
}

void SafeTrait::delete_iterator(BasicList::BasicIterator* iter) {
	List<BasicList::BasicIterator*>::Iterator i = iters.begin();
	for (; i != iters.end() && *i != iter; ++i);
	if (i != iters.end() && *i == iter) iters.erase(i); 
}

void SafeTrait::invalidate_iterators(BasicList::BasicIterator iter) {
	for (List<BasicList::BasicIterator*>::Iterator i = iters.begin(); i != iters.end(); ++i)
		if (**i == iter) (**i).invalidate();
}

void SafeTrait::invalidate_all() {
	for (List<BasicList::BasicIterator*>::Iterator i = iters.begin(); i != iters.end(); ++i) 
		(**i).invalidate();
}