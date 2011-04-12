#include "list.hpp"

void BasicList::init() {
	head = new BasicNode();
	tail = new BasicNode(head);
	head->next = tail;
	sz = 0;
}

void BasicList::destroy() {
	trait->invalidate_all();
	while (head != NULL) {
		BasicNode *tmp = head;
		head = head->next;
		delete tmp;
	}
}

void BasicList::clear() {
	destroy();
	init();
}

BasicList::~BasicList() {
	destroy();
	delete trait;
}

BasicList::BasicNode* BasicList::erase_node(BasicIterator pos) {
	assert(pos.node != NULL);
	trait->invalidate_iterators(pos);
	BasicNode *tmp = pos.node->prev->next = pos.node->next;
	pos.node->next->prev = pos.node->prev;
	delete pos.node;
	pos.node = NULL;
	sz--;
	return tmp;
}

BasicList::BasicNode* BasicList::insert_node(BasicIterator pos, BasicNode* node) {
	assert(pos.node != NULL);
	if (pos.node == head) throw BasicIterator::IteratorIsBeforeFirst();
	BasicNode *tmp = node;
	pos.node->prev->next = tmp;
	pos.node->prev = tmp;
	sz++;
	return tmp;
}