#ifndef HASH_MAP_HPP
#define HASH_MAP_HPP

#include <cstddef>
#include <cstring>

template<class KeyT, class ValT> class HashMap {
    static const size_t TABLESZ = 10;
    static const double HASH_MULTIPLIER;
    size_t sz; 
    size_t hashf(const KeyT& key) const;
public:
    struct Pair {
        KeyT key;
        ValT val;
        Pair(KeyT k, ValT v): key(k), val(v) {}
        Pair(const Pair& x): key(x.key), val(x.val) {}
    };
    typedef const char *rep_func(const void*);
private:
    struct Node {
        Pair v;
        Node *next;
        Node(Pair val, Node *n): v(val), next(n) {}
    };
    Node *table[TABLESZ];
    void copy(const HashMap& x);
    rep_func *repf;
public:
    class UnportableHashFunction {};
    HashMap(rep_func rf = NULL);
    HashMap(const HashMap& x);
    ~HashMap();
    HashMap& operator=(const HashMap& x);

    class Iterator {
        friend class HashMap;
        HashMap *map;
        size_t i;
        Node *node;
        Iterator(HashMap* m, size_t ind, Node *n): map(m), i(ind), node(n) {}
        void advance();
    public:
        class OutOfRange {};
        class IteratorIsNotDereferencable {};
        Iterator(const Iterator& x): map(x.map), i(x.i), node(x.node) {}
        Iterator& operator++();
        Iterator operator++(int);
        Pair& operator*() { if (node == NULL) throw IteratorIsNotDereferencable(); return node->v; }
        bool operator==(const Iterator& x) { return !(*this != x); }
        bool operator!=(const Iterator& x) { return node != x.node; }
    };
    Iterator begin() { Iterator i(this, 0, table[0]); if (!table[0]) i++; return i; }
    Iterator end() { Iterator i(this, TABLESZ, NULL); return i; }

    bool empty() { return !sz; }
    size_t size() { return sz; }

    ValT& operator[](const KeyT& key);

    void insert(const KeyT& key, const ValT& val);
    void erase(const KeyT& key);
    void clear();
};
template<class KeyT, class ValT> 
const double HashMap<KeyT, ValT>::HASH_MULTIPLIER = 0.31415926535;

template<class KeyT, class ValT>
size_t HashMap<KeyT, ValT>::hashf(const KeyT& key) const {
    const char *k;
    size_t len;
    if (repf != NULL) {
        k = repf(&key);
        len = strlen(k);
    } else {
        k = (const char*) &key;
        len = sizeof(KeyT);
    }
    unsigned int c = 0;
    for (size_t i = 0; i < len; i++)
        c += k[i]*(i + 1);
    double f = HASH_MULTIPLIER*c;
    return (size_t) ((f - (int) f)*(TABLESZ - 1));
}

template<class KeyT, class ValT>
void HashMap<KeyT, ValT>::copy(const HashMap& x) {
    sz = x.sz;
    for (size_t i = 0; i < TABLESZ; i++) {
        Node *t = x.table[i];
        Node *p;
        if (t != NULL) {
            p = table[i] = new Node(t->v, NULL);
            t = t->next; 
        } else
            table[i] = 0;
        while (t != NULL) {
            p = p->next = new Node(t->v, NULL);
            t = t->next;
        }
    }
}

template<class KeyT, class ValT>
HashMap<KeyT, ValT>::HashMap(rep_func rf): sz(0), repf(rf) {
    if (sizeof(unsigned char) != 1)
        throw UnportableHashFunction();
    for (size_t i = 0; i < TABLESZ; i++) 
        table[i] = NULL;
}

template<class KeyT, class ValT>
HashMap<KeyT, ValT>::HashMap(const HashMap& x): repf(x.repf) {
    copy(x);
}

template<class KeyT, class ValT>
HashMap<KeyT, ValT>::~HashMap() {
    clear();
}

template<class KeyT, class ValT>
HashMap<KeyT, ValT>& HashMap<KeyT, ValT>::operator=(const HashMap& x) {
    clear();
    copy(x);
    return *this;
}

template<class KeyT, class ValT>
void HashMap<KeyT, ValT>::Iterator::advance() {
    if (i == TABLESZ)
        throw OutOfRange();
    if (node != NULL)
        node = node->next;
    if (node == NULL) { 
        for (i = i + 1; i < TABLESZ && map->table[i] == NULL; i++);
        if (i == TABLESZ)
            node = NULL;
        else
            node = map->table[i];
    }
}

template<class KeyT, class ValT>
typename HashMap<KeyT, ValT>::Iterator& HashMap<KeyT, ValT>::Iterator::operator++() {
    advance();
    return *this;
}

template<class KeyT, class ValT>
typename HashMap<KeyT, ValT>::Iterator HashMap<KeyT, ValT>::Iterator::operator++(int) {
    Iterator x(*this);
    advance();
    return x;
}

template<class KeyT, class ValT>
ValT& HashMap<KeyT, ValT>::operator[](const KeyT& key) {
    size_t i = hashf(key);
    Node *t = table[i];    
    while (t != NULL && t->v.key != key)        
        t = t->next;        
    if (t != NULL)    
        return t->v.val;
    else {
        table[i] = new Node(Pair(key, ValT()), table[i]);
        sz++;
        return table[i]->v.val;
    }
}

template<class KeyT, class ValT>
void HashMap<KeyT, ValT>::insert(const KeyT& key, const ValT& val) {
    (*this)[key] = val;
}

template<class KeyT, class ValT>
void HashMap<KeyT, ValT>::erase(const KeyT& key) {
    size_t i = hashf(key);
    Node *t = table[i], *pt = NULL;    
    while (t != NULL && t->v.key != key) {
        pt = t;
        t = t->next;
    }
    if (t != NULL) {
        if (pt != NULL)
            pt->next = t->next;
        if (t == table[i])
            table[i] = NULL;
        delete t;
        sz--;
    }
}

template<class KeyT, class ValT>
void HashMap<KeyT, ValT>::clear() {
    for (size_t i = 0; i < TABLESZ; i++) {
        Node *t = table[i];
        while (t != NULL) {
            Node *tmp = t;
            t = t->next;
            delete tmp;
        }
        table[i] = NULL;
    }
    sz = 0;
}

#endif //HASH_MAP_HPP

