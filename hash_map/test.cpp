#include "hash_map.hpp"
#include "string/my_string.hpp"

#include <iostream>
using namespace std;

int testNum;

template<class T> bool eq(const T& a, const T& b) {
	return a == b;
}

template<class T, class U> bool eq(const HashMap<T, U>& aa, const HashMap<T, U>& bb) {
    HashMap<T, U> &a = const_cast<HashMap<T, U>&>(aa);
    HashMap<T, U> &b = const_cast<HashMap<T, U>&>(bb);
    if (a.size() != b.size())
        return false;
    for (typename HashMap<T, U>::Iterator i = a.begin(); i != a.end(); i++)
        if ((*i).val != b[(*i).key])
            return false;
    return true;
}

template<class T> void test(const T& actual, const T& expected, const char* errMsg, int line) {
	testNum++;
	clog << "Test " << testNum << ": ";
	if (eq(actual, expected))
		clog << "OK" << endl;		
	else 
		clog << "FAILED, " << errMsg << ", line " << line << endl;
}

const char *f(const void* p) {
    return ((String *) p)->c_str();
};

int main() {
    {
        HashMap<String, int> h(f);
        test(h.size(), (size_t) 0, "HashMap(rep_func)", __LINE__);
        test(h.empty(), true, "empty() true", __LINE__);        
        h["666"] = 823;
        test(h.empty(), false, "empty() false", __LINE__);        
        h["123"] = 123;
        test(h.size(), (size_t) 2, "size()", __LINE__);
        HashMap<String, int> h1(h);
        test(h, h1, "HashMap(const HashMap&)", __LINE__);
        h1["ababaca"] = 20;
        HashMap<String, int> h2(h);
        h2 = h1;
        test(h1, h2, "operator=(const HashMap&)", __LINE__);
        h2.clear();
        HashMap<String, int> h3(f);
        test(h2, h3, "clear()", __LINE__);
    }

    {
        HashMap<int, int> h;
        test(h.size(), (size_t) 0, "HashMap()", __LINE__);
        test(h.begin() == h.end(), true, "begin() == end() true", __LINE__);
        test(h.begin() != h.end(), false, "begin() != end() false", __LINE__);
        h[666];
        test(h.begin() == h.end(), false, "begin() == end() false", __LINE__);
        test(h.begin() != h.end(), true, "begin() != end() true", __LINE__);
        test(++h.begin() == h.end(), true, "Iterator::operator++()", __LINE__);
        test(h.begin()++ == h.end(), false, "Iterator::operator++(int)", __LINE__);
        HashMap<int, int>::Iterator i = h.begin();
        i++;
        test(i == h.end(), true, "Iterator::operator++(int)", __LINE__);
    }

    {
        HashMap<String, int> h(f);
        test(h["qwer"], 0, "operator[] insert", __LINE__);
        test(h.size(), (size_t) 1, "operator[] insert", __LINE__);
        test(h["qwer"] = 666, 666, "operator[] write", __LINE__);
        test(h["qwer"], 666, "operator[] read", __LINE__);
        h.insert("ababaca", 777);
        test(h.size(), (size_t) 2, "insert(const KeyT&, const ValT&)", __LINE__);
        test(h["ababaca"], 777, "insert(const KeyT&, const ValT&)", __LINE__);
        h.erase("ababaca");
        test(h.size(), (size_t) 1, "erase(const KeyT&)", __LINE__);
        test(h["ababaca"], 0, "erase(const KeyT&)", __LINE__);
    }
    return 0;
}

