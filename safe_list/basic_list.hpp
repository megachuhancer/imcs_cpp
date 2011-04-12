#ifndef BASIC_LIST_HPP
#define BASIC_LIST_HPP

#include <cstddef>
#include <cassert>

class BasicTrait;

class BasicList {
	size_t sz;
protected:
	struct BasicNode {
		BasicNode *prev, *next;
		BasicNode(BasicNode* p = NULL, BasicNode* n = NULL): prev(p), next(n) {}
		virtual ~BasicNode() {};
	};
public:
	class BasicIterator {
		friend class BasicList;
	protected:
		BasicNode *node;
		BasicTrait *trait;
		bool is_invalid;

		BasicIterator(BasicNode* n, BasicTrait* t): node(n), trait(t), is_invalid(false) {}
	public:
		void invalidate() { is_invalid = true; }

		class OutOfRange {};
		class IteratorIsNotDereferencable {};
		class IteratorIsBeforeFirst: IteratorIsNotDereferencable {};
		class IteratorIsInvalid: IteratorIsNotDereferencable {};

		BasicIterator(const BasicIterator& i): node(i.node), trait(i.trait), is_invalid(i.is_invalid) {}

		virtual bool is_dereferencable() const { return false; }

		bool operator==(const BasicIterator& i) { return node == i.node; }
		bool operator!=(const BasicIterator& i) { return node != i.node; }
	};
protected:
	BasicNode *head, *tail;
	BasicTrait *trait;

	void init();
	void destroy();

	BasicNode* insert_node(BasicIterator pos, BasicNode* node);
	BasicNode* erase_node(BasicIterator pos);
public:
	BasicList() { init(); }
	~BasicList();

	bool empty() const { return !sz; }
	size_t size() const { return sz; }
	void clear();
};

class BasicTrait {
public:
	virtual ~BasicTrait() {}
	virtual void add_iterator(BasicList::BasicIterator* iter) {}
	virtual void delete_iterator(BasicList::BasicIterator* iter) {}
	virtual void invalidate_iterators(BasicList::BasicIterator iter) {}
	virtual void invalidate_all() {}
};

#endif