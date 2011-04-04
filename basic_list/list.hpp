#ifndef LIST_HPP
#define LIST_HPP

#include "basic_list.hpp"

template<class T> class List: public BasicList {
public:
	class Iterator: public BasicIterator {
		friend class List<T>;
		Iterator(BasicNode *n): BasicIterator(n) {}
	public:
		Iterator(const BasicIterator& i): BasicIterator(i) {}

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
	List() {}
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

//----------------------------------------------------------------------------------------

template<class T> bool List<T>::Iterator::is_dereferencable() const { 
	if (node == NULL) return false;
	Node *n = dynamic_cast<Node*>(node);
	if (n) return n->data != NULL;
	else return false;
}

template<class T> typename List<T>::Iterator& List<T>::Iterator::operator++() { 
	if (node->next == NULL) throw OutOfRange(); 
	node = node->next; 
	return *this;
}

template<class T> typename List<T>::Iterator& List<T>::Iterator::operator--() { 
	if (node->prev == NULL) throw OutOfRange(); 
	node = node->prev; 
	return *this;
}

template<class T> T& List<T>::Iterator::operator*() {
	if (!is_dereferencable()) throw IteratorIsNotDereferencable();
	Node *n = dynamic_cast<Node*>(node);
	if (n) return *(n->data);
	else throw IteratorIsNotDereferencable();
}

//----------------------------------------------------------------------------------------

template<class T> void List<T>::assign_init(Iterator first, Iterator last) {
	init();
	for (; first != last; ++first) push_back(*first); 
}

template<class T> List<T>::List(size_t n, const T& x) {
	init();
	for (size_t i = 0; i < n; i++) push_back(x);
}

template<class T> List<T>::List(typename List<T>::Iterator first, typename List<T>::Iterator last) {
	assign_init(first, last);
}

template<class T> List<T>::List(const List<T>& l) {
	assign_init(l.begin(), l.end());
}

template<class T> List<T>& List<T>::operator=(const List<T>& l) {
	assign(l.begin(), l.end());
	return *this;
}

template<class T> typename List<T>::Iterator List<T>::begin() const {
	return Iterator(head->next);
}

template<class T> typename List<T>::Iterator List<T>::end() const {
	return Iterator(tail);
}

template<class T> const T& List<T>::front() const {
	return *begin();
}

template<class T> const T& List<T>::back() const {
	return *--end();
}

template<class T> void List<T>::assign(typename List<T>::Iterator first, typename List<T>::Iterator last) {
	destroy();
	assign_init(first, last);
} 

template<class T> void List<T>::push_front(const T& x) {
	insert(begin(), x);
}

template<class T> void List<T>::pop_front() {
	erase(begin());
}

template<class T> void List<T>::push_back(const T& x) {
	insert(end(), x);
}

template<class T> void List<T>::pop_back() {
	erase(--end());
}

template<class T> typename List<T>::Iterator List<T>::insert(typename List<T>::Iterator pos, const T& x) {
	return Iterator(insert_node(pos, new Node(pos.node->prev, pos.node, x)));
}

template<class T> void List<T>::insert(Iterator pos, size_t n, const T& x) {
	for (size_t i = 0; i < n; i++) insert(pos, x);
}

template<class T> void List<T>::insert(typename List<T>::Iterator pos, 
									   typename List<T>::Iterator first, 
									   typename List<T>::Iterator last) {
	for (; first != last; ++first) insert(pos, *first);
}

template<class T> typename List<T>::Iterator List<T>::erase(typename List<T>::Iterator pos) {
	if (!pos.is_dereferencable()) throw typename Iterator::IteratorIsNotDereferencable();
	return Iterator(erase_node(pos));
}

template<class T> typename List<T>::Iterator List<T>::erase(typename List<T>::Iterator first, typename List<T>::Iterator last) {
	while (first != last) first = erase(first);
	return last;
}

#endif