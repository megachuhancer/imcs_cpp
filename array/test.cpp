#include "array.hpp"
#include "string/my_string.hpp"

#include <iostream>
#include <ctime>
using namespace std;

int testNum;

template<class T> bool eq(const T& a, const T& b) {
	return a == b;
}

template<class T> bool eq(const Array<T>& a, const T* b, size_t size) {
	if (a.size() != size) return false;
	bool flg = true;
	size_t j = 0; 
	for (typename Array<T>::Iterator i = a.begin(); i != a.end(); ++i, ++j) 
		flg &= *i == b[j];
	return flg;
}

template<class T> bool eq(const Array<T>& a, const Array<T>& b) {
	if (a.size() != b.size()) return false;
	bool flg = true;
	for (typename Array<T>::Iterator i = a.begin(), j = b.begin(); i != a.end(); ++i, ++j) 
		flg &= *i == *j;
	return flg;
}

template<class T> void test(const T& actual, const T& expected, const char* errMsg, int line) {
	testNum++;
	clog << "Test " << testNum << ": ";
	if (eq(actual, expected))
		clog << "OK" << endl;		
	else 
		clog << "FAILED, " << errMsg << ", line " << line << endl;
}

template<class T> void test(const Array<T>& actual, const T* expected, size_t size, const char* errMsg, size_t line) {
	testNum++;
	clog << "Test " << testNum << ": ";
	if (eq(actual, expected, size))
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

template<class T> void tt(const char* name) {
    T a;
    clock_t begin = clock();
    for (int i = 0; i < 133; i++)
        a.push_back("ababaca");
    for (int i = 0; i < 133; i++) {
        a.pop_back();
        a.push_back("ababaca");
        a.pop_back();
    }
    clock_t end = clock();
    cout << "Time for " << name << " " << end - begin << endl;
}

#define t(a, e, m) test(a, e, m, __LINE__)
#define tl(a, e, s, m) test(a, e, s, m, __LINE__)
#define te(t, f, n) test_exception<t, f>(n, __LINE__)

struct Counter {
    static int cnt;
    Counter() { cnt++; }
    ~Counter() { cnt--; }
    void operator=(const Counter&) {}
};
int Counter::cnt = 0;

int main() {
    {
        t(Array<String>::Array().size(), (size_t) 0, "List()");
    }
    {
        String e[] = { "", "", "" };
        Array<String> a(3); 
        tl(a, e, 3, "Array(size_t)");
    }
    {
        String e[] = { "ab", "ab", "ab" };
        Array<String> a(3, "ab"); 
        tl(a, e, 3, "Array(size_t, const T&)");

        Array<String> b(a.begin(), a.end());
        t(b, a, "Array(Iterator, Iterator)");

        Array<String> c(a);
        t(c, a, "Array(const Array&)");

        Array<String> d;
        d = c;
        t(d, c, "operator=(const Array&)");
    }

    {
        String e[] = { "ab", "bc", "cd" };
        Array<String> a;
        a.push_back(e[0]);
        a.push_back(e[1]);
        a.push_back(e[2]);

        t(*a.begin(), e[0], "begin()");
        t(*--a.end(), e[2], "end()");

        t(a.size(), (size_t) 3, "size()");
        a.resize(666);
        t(a.size(), (size_t) 666, "resize(size_t) increase");
        String e1[] = { "ab", "bc" };
        a.resize(2);
        tl(a, e1, 2, "resize(size_t) decrease");
    }
    {
        Array<String> a;
        t(a.empty(), true, "empty() true");
        a.push_back("ababaca");
        t(a.empty(), false, "empty() false");
        a.reserve(666);
        t(a.capacity() >= 666, true, "reserve(size_t)");
        t(a[0], String("ababaca"), "operator[]");
        t(a.at(0), String("ababaca"), "at(size_t)");
    }
    {
        struct S {
            void operator()() {
                Array<String> a;
                a.at(1);
            }
        };
        te(Array<String>::OutOfRange, S, "at(size_t) - OutOfRange");
    }
    {
        Array<String> a;
        a.push_back("ababa");
        a.push_back("ca");
        t(a.front(), String("ababa"), "front()");
        t(a.back(), String("ca"), "back()");
    }

    {
        Array<String> a;
        a.push_back("a");
        a.push_back("b");
        String e[] = { "a", "b" };
        tl(a, e, 2, "push_back(const T&)");
        a.pop_back();
        String e1[] = { "a" };
        tl(a, e1, 1, "pop_back()");
        Array<String>::Iterator i = a.begin();
        i++;
        a.insert(i, "b");
        tl(a, e, 2, "insert(Iterator, const T&) end");
        i--;
        a.insert(i, "c");
        String e2[] = { "c", "a", "b" };
        tl(a, e2, 3, "insert(Iterator, const T&) begin");
        i++;
        a.insert(i, "d");
        String e3[] = { "c", "d", "a", "b" };
        tl(a, e3, 4, "insert(Iterator, const T&)");
        a.erase(i);
        tl(a, e2, 3, "erase(Iterator)");
        i--;
        a.erase(i);
        tl(a, e, 2, "erase(Iterator) begin");
        i++;
        a.erase(i);
        tl(a, e1, 1, "erase(Iterator) end");
        a.clear();
        t(a.empty(), true, "clear()");
    }
    {
        struct S {
            void operator()() {
                Array<String> a;
                a.pop_back();
            }
        };
        te(Array<String>::ContainerIsEmpty, S, "ContainerIsEmpty");
    }

    {
        {
            Array<Counter, MultiplyStrategy<Counter> > a;
            for (int i = 0; i < 11; i++) {
                for (int j = 0; j < 99; j++)
                    a.push_back(Counter());
                Array<Counter, MultiplyStrategy<Counter> >::Iterator i = a.begin();
                for (int j = 0; j < 88; j++) {
                    a.erase(i);
                }
            }
        }
        t(Counter::cnt, 0, "Destructors test");
    }

    tt<Array<String> >("BasicStrategy");
    tt<Array<String, MultiplyStrategy<String> > >("MultiplyStrategy");

    {
        Array<int> a;
        for (int i = 0; i < 666; i++)
            a.push_back(i);
        Array<int>::Iterator i = a.begin();
        t(*++i, 1, "Iterator::operator++()");
        t(*--i, 0, "Iterator::operator--()");
        i += 12;
        t(*i, 12, "Iterator::operator+=(int)");
        i -= 6;
        t(*i, 6, "Iterator::operator-=(int)");
        i++;
        t(*i, 7, "Iterator::operator++(int)");
        i--;
        t(*i, 6, "Iterator::operator--(int)");
        i += 10;
        t(*i, 16, "Iterator::operator+(int)");
        i -= 8;
        t(*i, 8, "Iterator::operator-(int)");
    }
    {
        Array<int> a;
        a.push_back(0);
        Array<int>::Iterator i1 = a.begin();
        Array<int>::Iterator i2 = a.begin();
        t(i1 == i2, true, "Iterator::operator==(Iterator) true");
        i1++;
        t(i1 == i2, false, "Iterator::operator==(Iterator) false");
        t(i1 != i2, true, "Iterator::operator!=(Iterator) true");
        i1--;
        t(i1 != i2, false, "Iterator::operator!=(Iterator) false");
    }
    {
        struct S {
            void operator()() {
                Array<int> a;
                *a.begin();
            }
        };
        te(Array<String>::Iterator::IteratorIsNotDereferencable, S, "IteratorIsNotDereferencable");
    }
    {
        struct S {
            void operator()() {
                Array<int> a;
                a.begin()++;
            }
        };
        te(Array<String>::OutOfRange, S, "Iterator - OutOfRange");
    }
	return 0;
}