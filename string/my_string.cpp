#include "my_string.hpp"

#include <cstring>
#include <cassert>

String String::concat(const char* a, size_t la, const char* b, size_t lb) {
	char *buf = new char[la + lb + 1];
	strcpy(buf, a);
	strcat(buf, b);
	String tmp(buf);
	delete[] buf;
	return tmp;
}

String::String() {
	cont = new Content("", 0);
}

String::String(const char* s) {
	assert(s != NULL);
	cont = new Content(s, strlen(s));
}

String::String(const char* s, size_t n) {
	cont = new Content(s, n);
}

String::String(const String& s) {
	assert(s.cont != NULL);
	s.cont->refCount++;
	cont = s.cont;
}

String::String(const String& s, size_t pos, size_t n) {
	cont = new Content(s.c_str() + pos, n);
}

String::String(size_t n, char c) {
	cont = new Content(n, c);
}

String::~String() {
	assert(cont != NULL);
	if(!--cont->refCount) delete cont;
}

String& String::operator=(const String& s) {
	assert(s.cont != NULL);
	s.cont->refCount++;
	if (!--cont->refCount) delete cont;
	cont = s.cont;
	return *this;
}

String& String::operator=(const char* s) {
	assert(s != NULL);
	assert(cont != NULL);
	if (cont->refCount == 1) cont->assign(s, strlen(s));
	else {
		cont->refCount--;
		cont = new Content(s, strlen(s));
	}
	return *this;
}

String& String::operator=(char c) {
	char *buf = new char[2];
	buf[0] = c; buf[1] = 0;
	*this = buf;
	delete[] buf;
	return *this;
}

const char* String::c_str() const {
	assert(cont != NULL);
	return cont->s;
}

size_t String::find(const String& s, size_t pos) const {
	return strstr(cont->s + pos, s.cont->s) - cont->s;
}

String::ProxyChar String::operator[](size_t pos) {
	assert(cont != NULL);
	if (pos >= cont->sz) throw OutOfRange();
	return ProxyChar(*this, pos);
}

String operator+(const String& a, const String& b) {
	assert(a.cont != NULL);
	assert(a.cont->s != NULL);
	assert(b.cont != NULL);
	assert(b.cont->s != NULL);
	return String::concat(a.cont->s, a.size(), b.cont->s, b.size());
}

String operator+(const String& a, const char* b) {
	assert(a.cont != NULL);
	assert(a.cont->s != NULL);
	assert(b != NULL);
	return String::concat(a.cont->s, a.size(), b, strlen(b));
}

String operator+(const char* a, const String& b) {
	assert(a != NULL);
	assert(b.cont != NULL);
	assert(b.cont->s != NULL);
	return String::concat(a, strlen(a), b.cont->s, b.size());
}

bool operator<(const String& a, const String& b) {
	return strcmp(a.cont->s, b.cont->s) == -1;
}

bool operator<(const String& a, const char* b) {
	return strcmp(a.cont->s, b) == -1;
}

bool operator<(const char* a, const String& b) {
	return strcmp(a, b.cont->s) == -1;
}

bool operator>(const String& a, const String& b) {
	return strcmp(a.cont->s, b.cont->s) == 1;
}

bool operator>(const String& a, const char* b) {
	return strcmp(a.cont->s, b) == 1;
}

bool operator>(const char* a, const String& b) {
	return strcmp(a, b.cont->s) == 1;
}

bool operator<=(const String& a, const String& b) {
	return !(a > b);
}

bool operator<=(const String& a, const char* b) {
	return !(a > b);
}

bool operator<=(const char* a, const String& b) {
	return !(a > b);
}

bool operator>=(const String& a, const String& b) {
	return !(a < b);
}

bool operator>=(const String& a, const char* b) {
	return !(a < b);
}

bool operator>=(const char* a, const String& b) {
	return !(a < b);
}

bool operator==(const String& a, const String& b) {
	assert(a.cont != NULL);
	assert(a.cont->s != NULL);
	assert(b.cont != NULL);
	assert(b.cont->s != NULL);
	return !strcmp(a.cont->s, b.cont->s);
}

bool operator==(const String& a, const char* b) {
	assert(a.cont != NULL);
	assert(a.cont->s != NULL);
	assert(b != NULL);
	return !strcmp(a.cont->s, b);
}

bool operator==(const char* a, const String& b) {
	return b == a;
}

bool operator!=(const String& a, const String& b) {
	return !(a == b);
}

bool operator!=(const String& a, const char* b) {
	return !(a == b);
}

bool operator!=(const char* a, const String& b) {
	return !(a == b);
}

std::ostream& operator<<(std::ostream& s, const String& str) {
	s << str.c_str();
	return s;
}