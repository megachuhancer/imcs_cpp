#ifndef LIST_H
#define LIST_H

#include <cstddef>
#include <cassert>

template<class T> class List {
public:
	class Iterator {
		friend class List<T>;

		typename List<T>::Node *node;

		Iterator(typename List<T>::Node* n): node(n) {}
	public:
		class OutOfRange {};
		class IteratorIsNotDereferencable {};
		class IteratorIsBeforeFirst: IteratorIsNotDereferencable {};

		Iterator(const Iterator& i): node(i.node) {}

		bool is_dereferencable() const { return node != NULL && node->data != NULL; }

		bool operator==(const Iterator& i) { return node == i.node; }
		bool operator!=(const Iterator& i) { return node != i.node; }

		Iterator& operator++();
		Iterator& operator--();
		T& operator*();
	};
private:
	struct Node {
		T *data;
		Node *prev, *next;

		Node(Node* p = NULL, Node* n = NULL): data(NULL), prev(p), next(n) {}
		Node(Node* p, Node* n, const T& d): prev(p), next(n) { data = new T(); *data = d; }
		~Node() { delete data; }
	};

	Node *head, *tail;
	size_t sz;

	void init(); 
	void destroy();
	void assign_init(Iterator first, Iterator last);
public:
	List() { init(); }
	List(size_t n, const T& x);
	List(Iterator first, Iterator last);
	List(const List<T>& l);

	~List();

	List<T>& operator=(const List<T>& l);

	Iterator begin() const;
	Iterator end() const;

	bool empty() const { return !sz; }
	size_t size() const { return sz; }

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
	void clear();
};

//----------------------------------------------------------------------------------------

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
	return *(node->data);
}

//----------------------------------------------------------------------------------------

template<class T> void List<T>::init() {
	head = new Node();
	tail = new Node(head);
	head->next = tail;
	sz = 0;
}

template<class T> void List<T>::destroy() {
	while (head != NULL) {
		Node *tmp = head;
		head = head->next;
		delete tmp;
	}
}

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

template<class T> List<T>::~List() {
	destroy();
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
	assert(pos.node != NULL);
	if (pos.node == head) throw typename Iterator::IteratorIsBeforeFirst();
	Node *tmp = new Node(pos.node->prev, pos.node, x);
	pos.node->prev->next = tmp;
	pos.node->prev = tmp;
	sz++;
	return Iterator(tmp);
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
	assert(pos.node != NULL);
	if (!pos.is_dereferencable()) throw typename Iterator::IteratorIsNotDereferencable();
	Node *tmp = pos.node->prev->next = pos.node->next;
	pos.node->next->prev = pos.node->prev;
	delete pos.node;
	pos.node = NULL;
	sz--;
	return Iterator(tmp);
}

template<class T> typename List<T>::Iterator List<T>::erase(typename List<T>::Iterator first, typename List<T>::Iterator last) {
	while (first != last) first = erase(first);
	return last;
}

template<class T> void List<T>::clear() {
	destroy();
	init();
}

#endif