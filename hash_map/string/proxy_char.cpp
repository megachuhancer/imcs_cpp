#include "my_string.hpp"

#include <cassert>

char String::ProxyChar::operator=(char c) {
	assert(s.cont != NULL);
	assert(s.cont->s != NULL);
	if (c != s.cont->s[p]) {
		if (s.cont->refCount == 1) s.cont->s[p] = c;
		else {
			s.cont->refCount--;
			s.cont = new Content(s.cont->s, s.cont->sz);
			s.cont->s[p] = c;
		}
	}
	return c;
}
