#ifndef HASH_MAP
#define HASH_MAP

#include <cstddef>
#include <cassert>
#include <algorithm>
#include <iostream>

template <typename T>
struct ListItem {
    ListItem(const T &v, ListItem<T> *p = nullptr, 
                      ListItem<T> *n = nullptr): 
                                        value(v),
                                        prev(p),
                                        next(n)
    {}
    ListItem<T> *prev;
    ListItem<T> *next;
    T value;
};

template <typename T>
class LinkedList {
public:

    LinkedList(): m_tail(nullptr),
                  m_head(nullptr)
    {}
    
    // performs a deep copy of whole list
    LinkedList(const LinkedList<T> &other) {
        m_head = m_tail = nullptr;
        ListItem<T> *i = other.m_head;
        ListItem<T> *prev = nullptr;
        while(i) {
            ListItem<T> *j = new ListItem<T>(i->value);
            if(!m_head) {
                m_head = j;
            }
            if(prev) {
                prev->next = j;
            }
            j->prev = prev;
            m_tail = prev = j;
            i = i->next;
        }
    }

    ~LinkedList() {
        clear();
    }

    void clear() {
        if(!m_head) {
            return;
        }

        ListItem<T> *i = m_head;
        ListItem<T> *tmp = nullptr;
        while(i) {
            tmp = i->next;
            delete i;
            i = tmp;
        }
        m_head = nullptr;
        m_tail = nullptr;
    }

    std::size_t size() const {
        if(!m_head) {
            return 0;
        }
        std::size_t n = 0;
        ListItem<T> *i = m_head;
        do {
            n++;
        } while((i = i->next));
        return n;
    }

    void pushBack(const T &val) {
        if(!m_head) {
            m_head = new ListItem<T>(val);
            m_tail = m_head;
        } else {
            m_tail->next = new ListItem<T>(val);  
            m_tail->next->prev = m_tail;
            m_tail = m_tail->next;
        }
    }
    
    ListItem<T> *find(const T &val) {
        ListItem<T> *i = m_head;
        while(i) {
            if(i->val == val) {
                return i;
            }
            i = i->next;
        }
        return nullptr;
    }
    
    void remove(ListItem<T> *i) {
        if(!i) {
            return;
        }
        ListItem<T> *prev = i->prev;
        ListItem<T> *next = i->next;
        if(prev) {
            prev->next = next;
        }
        if(next) {
            next->prev = prev;
        }
        delete i;
    }

    T &get(std::size_t n) const {
        ListItem<T> *i = m_head;
        while(i) {
            if(!n--) {
                return i->value;
            }
            i = i->next;
        }
    }

    T &operator[](std::size_t n) const {
        return get(n);
    }

    T &tail() const {
        assert(m_tail);
        return m_tail->value;
    }

    void print() const {
        ListItem<T> *i = m_head;
        while(i) {
            i = i->next;
        }
    }

private:
    ListItem<T> *m_head;
    ListItem<T> *m_tail;

};

template <typename K, typename V>
struct KeyVal {
    KeyVal(K k = K{}, const V v = V{}): key(k), value(v)
    {}

    std::ostream &write(std::ostream &s) const {
        s << key << " : " << value;
    }

    K key;
    V value;
};

template <typename K, typename V>
std::ostream &operator<<(std::ostream &s, const KeyVal<K, V> &kv) {
    kv.write(s);
}

template <typename K, typename V>
class BucketPool {
public:
    BucketPool(std::size_t size): m_pool(nullptr), m_poolSize(0)
    {
        resize(size);
    }

    ~BucketPool() {
        clear();
    }
    
    void clear() {
        if(!m_pool) {
            return;
        }

        delete [] m_pool;
        m_pool = nullptr;
        m_poolSize = 0;
    }


    LinkedList<KeyVal<K, V>> &get(std::size_t n) {
        assert(n<m_poolSize);
        return m_pool[n];
    }

    LinkedList<KeyVal<K, V>> &operator[](std::size_t n) {
        return get(n);
    }

    void resize(std::size_t newSize) {
        if(!m_pool) {
            m_pool = new LinkedList<KeyVal<K, V>>[newSize];
            m_poolSize = newSize;
        } else {
            LinkedList<KeyVal<K, V>> *newPool = new LinkedList<KeyVal<K, V>>[newSize];
            if(newSize>m_poolSize) {
                std::copy(m_pool, m_pool+m_poolSize, newPool);
            } else {
                std::copy(m_pool, m_pool+newSize, newPool);
            }
            delete [] m_pool;
            m_pool = newPool;
            m_poolSize = newSize;
        }
    }

    std::size_t size() const {
        return m_poolSize;
    }

private:
    LinkedList<KeyVal<K, V>> *m_pool;
    std::size_t m_poolSize;
};

/**
 * @biref A generic hash function
 *
 * Used by @HashMap.
 * Template specifications
 * should be provided for
 * for every required type.
 */
template <typename T> std::size_t hash(const T&);

/**
 * @brief Hash function for std::string
 *
 * Uses djb2 hashing algorithm.
 * http://www.cse.yorku.ca/~oz/hash.html
 */
template <>
std::size_t hash(const std::string &str) {
    std::size_t h = 5328;

    for(auto ch : str) {
        h = ((h << 5) + h) + ch;
    }
    return h;
}

/**
 * @brief Hash function for std::int32_t
 *
 * Uses multiplication method;
 * http://lcm.csa.iisc.ernet.in/dsa/node44.html
 */
template <>
std::size_t hash(const std::int32_t &n) {
    static const double goldenRatio = 0.6180339887;
    double fractionPart = n*goldenRatio - std::size_t(n*goldenRatio);
    return std::size_t(fractionPart*(2 << 30));
}

/**
 * @brief Hash function for std::uint32_t
 *
 * Uses multiplication method;
 * http://lcm.csa.iisc.ernet.in/dsa/node44.html
 */
template <>
std::size_t hash(const std::uint32_t &n) {
    static const double goldenRatio = 0.6180339887;
    double fractionPart = n*goldenRatio - std::size_t(n*goldenRatio);
    return std::size_t(fractionPart*(2 << 30));
}

template <typename K, typename V>
class HashMap {
public:
    HashMap( std::size_t capacity = 16, float loadFactor = 0.75): 
                                        m_buckets(capacity),
                                        m_loadFactor(loadFactor)
    {}

    std::size_t capacity() const {
        return m_buckets.size();
    }

    std::size_t size() {
        std::size_t n = 0;
        for(int i = 0;i<m_buckets.size();i++) {
            n+=m_buckets[i].size();
        }
        return n;
    }

    float getLoad() {
        return (float)size()/capacity();
    }

    V &get(const K &k) {
        std::size_t h = hash(k)%capacity();
        auto &list = m_buckets[h];
        for(int j = 0;j<list.size();j++) {
            if(list[j].key == k) {
                return list[j].value;
            }
        }

        list.pushBack(KeyVal<K, V>(k, V{}));
        if((float)size()/capacity()>=m_loadFactor) {
            rehash();
            std::size_t newH = hash(k)%capacity();
            auto &newList = m_buckets[newH];
            return newList.tail().value;
        }

        return list.tail().value;
    }

    V &operator[](const K &k) {
        return get(k);
    }

    void debugPrint() {
        for(int i = 0;i<m_buckets.size();i++) {
            for(int j = 0;j<m_buckets[i].size();j++) {
                std::cerr << m_buckets[i][j] << std::endl;
            }
        }
    }

private:
    /**
     * @brief Increases bucket count and
     * performs complete rehashing of data
     */
    void rehash() {
        KeyVal<K, V> *tempStorage = new KeyVal<K, V>[size()];
        std::size_t n = 0;
        for(int i = 0;i<m_buckets.size();i++) {
            auto &b = m_buckets[i];
            for(int j = 0;j<b.size();j++) {
                tempStorage[n++] = b[j];
            }
        }

        std::size_t numBuckets = m_buckets.size();
        m_buckets.clear();
        m_buckets.resize(numBuckets*2);

        for(int i = 0;i<n;i++) {
            std::size_t h = hash(tempStorage[i].key)%capacity();
            auto &bucket = m_buckets[h];
            bucket.pushBack(tempStorage[i]);
        }
    }

private:
    BucketPool<K, V> m_buckets;
    float m_loadFactor;
};

#endif // HASH_MAP
