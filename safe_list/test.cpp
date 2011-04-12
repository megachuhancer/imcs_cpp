#include "list.hpp"

#include <iostream>
using namespace std;

int testNum;

template<class T> bool cmp(const T& a, const T& b) {
	return a == b;
}

template<class T> bool cmp(const List<T>& a, const T* b, size_t size) {
	if (a.size() != size) return false;
	bool flg = true;
	size_t j = 0; 
	for (typename List<T>::Iterator i = a.begin(); i != a.end(); ++i, ++j) 
		flg &= *i == b[j];
	return flg;
}

template<class T> bool cmp(const List<T>& a, const List<T>& b) {
	if (a.size() != b.size()) return false;
	bool flg = true;
	for (typename List<T>::Iterator i = a.begin(), j = b.begin(); i != a.end(); ++i, ++j) 
		flg &= *i == *j;
	return flg;
}

template<class T> void test(const T& actual, const T& expected, const char* errMsg, int line) {
	testNum++;
	clog << "Test " << testNum << ": ";
	if (cmp(actual, expected))
		clog << "OK" << endl;		
	else 
		clog << "FAILED, " << errMsg << ", line " << line << endl;
}

template<class T> void test(const List<T>& actual, const T* expected, size_t size, const char* errMsg, size_t line) {
	testNum++;
	clog << "Test " << testNum << ": ";
	if (cmp(actual, expected, size))
		clog << "OK" << endl;		
	else 
		clog << "FAILED, " << errMsg << ", line " << line << endl;
}

template<class T, class F> void test_exception(char* name, size_t line) {
	bool caught = false;
	try {
		F()();
	} catch(T) {
		caught = true;
		clog << "OK: " << name << " exception caught" << endl;
	} catch(...) {
		clog << "FAIL: unexpected exception caught, line " << line << endl;
	}
	if (!caught) clog << "FAIL: " << name << " exception expected, line " << line << endl;
}

#define t(a, e, m) test(a, e, m, __LINE__)
#define tl(a, e, s, m) test(a, e, s, m, __LINE__)
#define te(t, f, n) test_exception<t, f>(n, __LINE__)

int main() {
	t(List<int>::List().size(), (size_t) 0, "List()");
	{
		int a[] = { 6, 6, 6 };
		List<int> l(3, 6);
		tl(l, a, 3, "List(size_t, const T&)");
		t(List<int>::List(l), List<int>::List(l.begin(), l.end()), "List(const List&), List(Iterator, Iterator)");
		List<int> l1 = l;
		t(l1, l, "operator=");
	}
	
	t(*List<int>::List(6, 7).begin(), 7, "begin()");
	t(*--List<int>::List(6, 9).end(), 9, "end()");

	t(List<int>::List().empty(), true, "empty()");
	t(List<int>::List(666, 0).size(), (size_t) 666, "size()");

	t(List<int>::List(1, 5).front(), 5, "front()");
	t(List<int>::List(2, 10).back(), 10, "end()");

	{
		List<int> l;
		for (int i = 0; i < 7; i++) l.push_back(i);
		List<int> l1(2, 66);
		l1.assign(++l.begin(), --l.end());
		int a[] = { 1, 2, 3, 4, 5 };
		tl(l1, a, 5, "assign(Iterator, Iterator)");
		l1.push_front(0);
		int b[] = { 0, 1, 2, 3, 4, 5 };
		tl(l1, b, 6, "push_front(const T&)");
		l1.pop_front();
		tl(l1, a, 5, "pop_front()");
		l1.push_back(0);
		int c[] = { 1, 2, 3, 4, 5, 0 };
		tl(l1, c, 6, "push_back(const T&)");
		l1.pop_back();
		tl(l1, a, 5, "pop_back()");
	}

	{
		List<int> l(3, 6);
		List<int>::Iterator i = l.insert(++l.begin(), 4);
		int a[] = { 6, 4, 6, 6 };
		tl(l, a, 4, "insert(Iterator, const T&)");
		List<int> l1 = l;
		l.insert(i, 2, 10);
		int b[] = { 6, 10, 10, 4, 6, 6 };
		tl(l, b, 6, "insert(Iterator, size_t, const T&)");
		l.insert(i, ++l1.begin(), --l1.end());
		int c[] = { 6, 10, 10, 4, 6, 4, 6, 6 };
		tl(l, c, 8, "insert(Iterator, Iterator, Iterator)");
		i = l.erase(i);
		int d[] = { 6, 10, 10, 4, 6, 6, 6 };
		tl(l, d, 7, "erase(Iterator)");
		l.erase(i, l.end());
		int e[] = { 6, 10, 10, 4, 6 };
		tl(l, e, 5, "erase(Iterator, Iterator)");
		l.clear();
		t(l.size(), (size_t) 0, "clear()");
	}

	{
		struct S {
			void operator()() {
				List<int> l(6, 6);
				for (List<int>::Iterator i = l.begin(); ; ++i);
			}
		};
		te(BasicList::BasicIterator::OutOfRange, S, "OutOfRange");
	}

	{
		struct S {
			void operator()() {
				*List<int>(6, 6).end();
			}
		};
		te(BasicList::BasicIterator::IteratorIsNotDereferencable, S, "IteratorIsNotDereferencable");
	}

	{
		struct S {
			void operator()() {
				List<int> l(6, 6);
				l.insert(--l.begin(), 1);
			}
		};
		te(BasicList::BasicIterator::IteratorIsBeforeFirst, S, "IteratorIsBeforeFirst");
	}

	//List<int> l;
	//List<int>::Iterator i = l.begin();
	//i.invalidate();

	{
		struct S {
			void operator()() {
				List<int, SafeTrait> l(2, 666);
				List<int>::Iterator i = l.begin();
				List<int>::Iterator j = l.begin();
				l.erase(i);
				*j;
			}
		};
		te(BasicList::BasicIterator::IteratorIsInvalid, S, "IteratorIsInvalid");
	}
	return 0;
}