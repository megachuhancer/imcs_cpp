#ifndef LIST_HPP
#define LIST_HPP

#include "basic_list.hpp"

template<class T, class Trait = BasicTrait> class List: public BasicList {
public:
	class Iterator: public BasicIterator {
		friend class List<T, Trait>;

		Iterator(BasicNode* n, BasicTrait* t): BasicIterator(n, t) { trait->add_iterator(this); }
		using BasicIterator::invalidate;
	public:
		Iterator(const BasicIterator& i): BasicIterator(i) { trait->add_iterator(this); }
		~Iterator() { trait->delete_iterator(this); }

		bool is_dereferencable() const;

		Iterator& operator++();
		Iterator& operator--();
		T& operator*();
	};
private:
	struct Node: BasicNode {
		T *data;

		Node(BasicNode* p = NULL, BasicNode* n = NULL): BasicNode(p, n), data(NULL) {}
		Node(BasicNode* p, BasicNode* n, const T& d): BasicNode(p, n) { data = new T(); *data = d; }
		~Node() { delete data; }
	};

	void assign_init(Iterator first, Iterator last);
public:
	List() { trait = new Trait(); }
	List(size_t n, const T& x);
	List(Iterator first, Iterator last);
	List(const List<T>& l);

	List<T>& operator=(const List<T>& l);

	Iterator begin() const;
	Iterator end() const;

	const T& front() const;
	const T& back() const;

	void assign(Iterator first, Iterator last);
	void push_front(const T& x);
	void pop_front();
	void push_back(const T& x);
	void pop_back();
	Iterator insert(Iterator pos, const T& x);
	void insert(Iterator pos, size_t n, const T& x);
	void insert(Iterator pos, Iterator first, Iterator last);
	Iterator erase(Iterator pos);
	Iterator erase(Iterator first, Iterator last);
};

class SafeTrait: public BasicTrait {
	List<BasicList::BasicIterator*> iters;
public:
	~SafeTrait() {}
	void add_iterator(BasicList::BasicIterator* iter);
	void delete_iterator(BasicList::BasicIterator* iter);
	void invalidate_iterators(BasicList::BasicIterator iter);
	void invalidate_all();
};

//----------------------------------------------------------------------------------------

template<class T, class Trait> bool List<T, Trait>::Iterator::is_dereferencable() const { 
	if (node == NULL || is_invalid) return false;
	Node *n = dynamic_cast<Node*>(node);
	if (n) return n->data != NULL;
	else return false;
}

template<class T, class Trait> typename List<T, Trait>::Iterator& List<T, Trait>::Iterator::operator++() { 
	if (is_invalid) throw BasicIterator::IteratorIsInvalid();
	if (node->next == NULL) throw OutOfRange(); 
	node = node->next; 
	return *this;
}

template<class T, class Trait> typename List<T, Trait>::Iterator& List<T, Trait>::Iterator::operator--() { 
	if (is_invalid) throw BasicIterator::IteratorIsInvalid();
	if (node->prev == NULL) throw OutOfRange(); 
	node = node->prev; 
	return *this;
}

template<class T, class Trait> T& List<T, Trait>::Iterator::operator*() {
	if (is_invalid) throw BasicIterator::IteratorIsInvalid();
	if (!is_dereferencable()) throw IteratorIsNotDereferencable();
	Node *n = dynamic_cast<Node*>(node);
	if (n) return *(n->data);
	else throw IteratorIsNotDereferencable();
}

//----------------------------------------------------------------------------------------

template<class T, class Trait> void List<T, Trait>::assign_init(Iterator first, Iterator last) {
	init();
	for (; first != last; ++first) push_back(*first); 
}

template<class T, class Trait> List<T, Trait>::List(size_t n, const T& x) {
	trait = new Trait();
	init();
	for (size_t i = 0; i < n; i++) push_back(x);
}

template<class T, class Trait> List<T, Trait>::List(typename List<T, Trait>::Iterator first, 
													typename List<T, Trait>::Iterator last) {
	trait = new Trait();
	assign_init(first, last);
}

template<class T, class Trait> List<T, Trait>::List(const List<T>& l) {
	trait = new Trait();
	assign_init(l.begin(), l.end());
}

template<class T, class Trait> List<T>& List<T, Trait>::operator=(const List<T>& l) {
	assign(l.begin(), l.end());
	return *this;
}

template<class T, class Trait> typename List<T, Trait>::Iterator List<T, Trait>::begin() const {
	return Iterator(head->next, trait);
}

template<class T, class Trait> typename List<T, Trait>::Iterator List<T, Trait>::end() const {
	return Iterator(tail, trait);
}

template<class T, class Trait> const T& List<T, Trait>::front() const {
	return *begin();
}

template<class T, class Trait> const T& List<T, Trait>::back() const {
	return *--end();
}

template<class T, class Trait> void List<T, Trait>::assign(typename List<T, Trait>::Iterator first, 
														   typename List<T, Trait>::Iterator last) {
	destroy();
	assign_init(first, last);
} 

template<class T, class Trait> void List<T, Trait>::push_front(const T& x) {
	insert(begin(), x);
}

template<class T, class Trait> void List<T, Trait>::pop_front() {
	erase(begin());
}

template<class T, class Trait> void List<T, Trait>::push_back(const T& x) {
	insert(end(), x);
}

template<class T, class Trait> void List<T, Trait>::pop_back() {
	erase(--end());
}

template<class T, class Trait> typename List<T, Trait>::Iterator List<T, Trait>::insert(typename List<T, Trait>::Iterator pos, 
																						const T& x) {
	return Iterator(insert_node(pos, new Node(pos.node->prev, pos.node, x)), trait);
}

template<class T, class Trait> void List<T, Trait>::insert(Iterator pos, size_t n, const T& x) {
	for (size_t i = 0; i < n; i++) insert(pos, x);
}

template<class T, class Trait> void List<T, Trait>::insert(typename List<T, Trait>::Iterator pos, 
														   typename List<T, Trait>::Iterator first, 
														   typename List<T, Trait>::Iterator last) {
	for (; first != last; ++first) insert(pos, *first);
}

template<class T, class Trait> typename List<T, Trait>::Iterator List<T, Trait>::erase(typename List<T, Trait>::Iterator pos) {
	if (!pos.is_dereferencable()) throw BasicIterator::IteratorIsNotDereferencable();
	return Iterator(erase_node(pos), trait);
}

template<class T, class Trait> typename List<T, Trait>::Iterator List<T, Trait>::erase(typename List<T, Trait>::Iterator first, 
																					   typename List<T, Trait>::Iterator last) {
	while (first != last) first = erase(first);
	return last;
}

#endif