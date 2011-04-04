#ifndef BASIC_LIST_HPP
#define BASIC_LIST_HPP

#include <cstddef>
#include <cassert>

class BasicList {
	size_t sz;
protected:
	struct BasicNode {
		BasicNode *prev, *next;
		BasicNode(BasicNode* p = NULL, BasicNode* n = NULL): prev(p), next(n) {}
	private:
		virtual void f() {};
	};

	class BasicIterator {
		friend class BasicList;
	protected:
		BasicNode *node;
		BasicIterator(BasicNode* n): node(n) {}
	public:
		class OutOfRange {};
		class IteratorIsNotDereferencable {};
		class IteratorIsBeforeFirst: IteratorIsNotDereferencable {};

		BasicIterator(const BasicIterator& i): node(i.node) {}

		virtual bool is_dereferencable() const { return false; }

		bool operator==(const BasicIterator& i) { return node == i.node; }
		bool operator!=(const BasicIterator& i) { return node != i.node; }
	};
	
	BasicNode *head, *tail;

	void init();
	void destroy();

	BasicNode* insert_node(BasicIterator pos, BasicNode* node);
	BasicNode* erase_node(BasicIterator pos);
public:
	BasicList() { init(); }
	~BasicList() { destroy(); }

	bool empty() const { return !sz; }
	size_t size() const { return sz; }
	void clear();
};

#endif