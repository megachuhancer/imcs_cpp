#include "my_string.hpp"

#include <iostream>
#include <cstring>
using namespace std;

int testNum;

template<class T> bool cmp(const T& a, const T& b) {
	return a == b;
}

template<class T> void test(const T& actual, const T& expected, const char* errMsg, int line) {
	testNum++;
	clog << "Test " << testNum << ": ";
	if (cmp(actual, expected))
		clog << "OK" << endl;		
	else 
		clog << "FAILED, " << errMsg << ", line " << line << endl;
}

#define t(a, e, m) test(a, e, m, __LINE__)
#define ts(a, e, m) test<String>(a, e, m, __LINE__)

int main() {
	t(String("ababaca") == String("ababaca"), true, "String == String(true)");
	t(String("ababaca") == String("abaca"), false, "String == String(false)");
	t(String("ababaca") != String("abaca"), true, "String != String(true)");
	t(String("ababaca") != String("ababaca"), false, "String != String(false)");

	t(String("ababaca") == "ababaca", true, "String == char*(true)");
	t(String("ababaca") == "abaca", false, "String == char*(false)");
	t(String("ababaca") != "abaca", true, "String != char*(true)");
	t(String("ababaca") != "ababaca", false, "String != char*(false)");

	t("ababaca" == String("ababaca"), true, "char* == String(true)");
	t("ababaca" == String("abaca"), false, "char* == String(false)");
	t("ababaca" != String("abaca"), true, "char* != String(true)");
	t("ababaca" != String("ababaca"), false, "char* != String(false)");

	t(String("aba") < String("abb"), true, "String < String(true)");
	t(String("abb") < String("aba"), false, "String < String(false)");
	t(String("aba") <= String("aba"), true, "String <= String(true ==)");
	t(String("aba") <= String("abb"), true, "String <= String(true <)");
	t(String("abb") <= String("aba"), false, "String <= String(false)");
	t(String("abb") > String("aba"), true, "String > String(true)");
	t(String("aba") > String("abb"), false, "String > String(false)");
	t(String("aba") >= String("aba"), true, "String >= String(true ==)");
	t(String("abb") >= String("aba"), true, "String >= String(true >)");
	t(String("aba") >= String("abb"), false, "String >= String(false)");

	t(String("aba") < "abb", true, "String < const char*(true)");
	t(String("abb") < "aba", false, "String < const char*(false)");
	t(String("aba") <= "aba", true, "String <= const char*(true ==)");
	t(String("aba") <= "abb", true, "String <= const char*(true <)");
	t(String("abb") <= "aba", false, "String <= const char*(false)");
	t(String("abb") > "aba", true, "String > const char*(true)");
	t(String("aba") > "abb", false, "String > const char*(false)");
	t(String("aba") >= "aba", true, "String >= const char*(true ==)");
	t(String("abb") >= "aba", true, "String >= const char*(true >)");
	t(String("aba") >= "abb", false, "String >= const char*(false)");

	t("aba" < String("abb"), true, "const char* < String(true)");
	t("abb" < String("aba"), false, "const char* < String(false)");
	t("aba" <= String("aba"), true, "const char* <= String(true ==)");
	t("aba" <= String("abb"), true, "const char* <= String(true <)");
	t("abb" <= String("aba"), false, "const char* <= String(false)");
	t("abb" > String("aba"), true, "const char* > String(true)");
	t("aba" > String("abb"), false, "const char* > String(false)");
	t("aba" >= String("aba"), true, "const char* >= String(true ==)");
	t("abb" >= String("aba"), true, "const char* >= String(true >)");
	t("aba" >= String("abb"), false, "const char* >= String(false)");

	ts(String(), "", "String()");
	ts(String("qwe"), "qwe", "String(const char*)");
	ts(String("qwerty", 3), "qwe", "String(const char*, size_t)");
	ts(String(String("qwe")), "qwe", "String(const String&)");
	ts(String("ababacergreyshuistfoobarrior", 8, 10), "greyshuist", "String(const char*, size_t, size_t)");
	ts(String(10, 'q'), "qqqqqqqqqq", "String(size_t, char)");

	{
		String s("test");
		ts(s = String("666"), "666", "String = String");
		ts(s = "qwe", "qwe", "String = const char*");
		ts(s = 'q', "q", "String = char");

		t(s.size(), (size_t) 1, "size()");
		t(s.length(), (size_t) 1, "length()");
		s.clear();
		ts(s, "", "clear()");
		t(s.empty(), true, "empty()(true)");
		t(String("qwe").empty(), false, "empty()(false)");

		bool caught = false;
		try {
			s[0];
		} catch(String::OutOfRange) {
			clog << "OK: OutOfRange exception caught" << endl;
			caught = true;
		} catch(...) {
			clog << "FAIL: unexpected exception caught" << ", line " << __LINE__ << endl;
		}
		if(!caught) clog << "FAIL: OutOfRange exception expected" << ", line " << __LINE__ << endl;
	}
	t((char) String("q")[0], 'q', "[]");
	{
		String s("123");
		t(s[1] = '4', '4', "[]=(ret value)");
		t((char) s[1], '4', "[]=(new value)");
		ts(s, "143", "[]=(whole str)");
	}
	t(strcmp(String("qwe").c_str(), "qwe"), 0, "c_str()");
	t(String("qwqwerty").find("we", 1), (size_t) 3, "find()");
	ts(String("qwe") + String("rty"), "qwerty", "String + String");
	ts(String("qwe") + "rty", "qwerty", "String + const char*");
	ts("qwe" + String("rty"), "qwerty", "const char* + String");

	{
		String a, b;
		a = b = "qweasd";
		a[2] = 'x';
		t(a == "qwxasd" && b == "qweasd", true, "Copy-on-write");

		bool ok = true;
		try {
			a = a;
		} catch(...) {
			ok = false;
			clog << "FAIL: Self assignment, line " << __LINE__ << endl;
		}
		if (ok) clog << "OK: Self assignment" << endl;
	}

	clog << String("Testing complete");
	return 0;
}