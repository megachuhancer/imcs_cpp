#include "my_string.hpp"

#include <cstring>
#include <cassert>

void String::Content::allocMem(size_t n) {
	s = new char[n + 1];
	sz = n;
}

void String::Content::init(const char* p, size_t n) {
	assert(p != NULL);
	allocMem(n);
	strncpy(s, p, n);
	s[sz] = 0;
}

String::Content::Content(const char *p, size_t n): refCount(1) {
	init(p, n);
}

String::Content::Content(size_t n, char c): refCount(1) {
	allocMem(n);
	for(size_t i = 0; i < n; i++) s[i] = c;
	s[n] = 0;
}

String::Content::~Content() {
	delete[] s;
}

void String::Content::assign(const char *p, size_t n) {
	delete[] s;
	init(p, n);
}
