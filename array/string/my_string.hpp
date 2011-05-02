#ifndef MY_STRING_H
#define MY_STRING_H

#include <ostream>
#include <istream>

class String {
private:
	class ProxyChar;

	struct Content {
		char *s;
		size_t sz, refCount;

		Content(const char*, size_t);
		Content(size_t, char);
		~Content();

		void assign(const char *, size_t);
	private:
		void init(const char*, size_t);
		void allocMem(size_t);
	};

	Content *cont;

	static String concat(const char*, size_t, const char*, size_t);
public:
	class OutOfRange {};
	//---------------------------------
	String();
	String(const char*);
	String(const char*, size_t);
	String(const String&);
	String(const String&, size_t, size_t);
	String(size_t, char);
	//---------------------------------
	~String();
	//---------------------------------
	String& operator=(const String&);
	String& operator=(const char*);
	String& operator=(char);
	//---------------------------------
	size_t size() const { return cont->sz; }
	size_t length() const { return cont->sz; }
	void clear() { *this = ""; }
	bool empty() const { return cont->sz == 0; }
	//---------------------------------
	ProxyChar operator[](size_t); 
	//---------------------------------
	const char* c_str() const;
	size_t find(const String&, size_t) const;
	//---------------------------------
	friend String operator+(const String&, const String&);
	friend String operator+(const String&, const char*); 
	friend String operator+(const char*, const String&); 
	friend bool operator<(const String&, const String&);
	friend bool operator<(const String&, const char*);
	friend bool operator<(const char*, const String&);
	friend bool operator>(const String&, const String&);
	friend bool operator>(const String&, const char*);
	friend bool operator>(const char*, const String&);
	friend bool operator==(const String&, const String&);
	friend bool operator==(const String&, const char*);
};

bool operator<=(const String&, const String&);
bool operator<=(const String&, const char*);
bool operator<=(const char*, const String&);
bool operator>=(const String&, const String&);
bool operator>=(const String&, const char*);
bool operator>=(const char*, const String&);
bool operator==(const char*, const String&);
bool operator!=(const String&, const String&);
bool operator!=(const String&, const char*);
bool operator!=(const char*, const String&);
std::ostream& operator<<(std::ostream&, const String&);

class String::ProxyChar {
	String& s;
	size_t p;
public:
	ProxyChar(String& str, size_t pos): s(str), p(pos) {};
	char operator=(char);
	operator char() const { return s.cont->s[p]; }
};

#endif