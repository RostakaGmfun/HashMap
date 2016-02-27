#ifndef HASH_MAP
#define HASH_MAP

/**
 * @file HashMap.hpp
 * @author Rostyslav Kurylo
 * @brief A dynamic hash map implementation
 *
 * The below implementation uses @LinkedList
 * to represent hash map buckets.
 * The buckets itself are held in dynamic
 * array implemented by @Array template class.
 * The @LinkedList and @Array containers
 * are not complete implementations of
 * underlying data structures and implement
 * only operations required by @HashMap.
 */

#include <cstdint>
#include <cstring>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>

/**
 * @biref Template class which represents 
 * single element of linked list.
 */
template <typename T>
struct ListItem {
    ListItem(const T &v): value(v),
                          prev(nullptr), 
                          next(nullptr)
    {}

    T value;
    ListItem *prev;
    ListItem *next;
};

/**
 * @brief Iterator class for @LinkedList
 */
template <typename T>
class LinkedListIterator: public std::iterator<std::forward_iterator_tag, ListItem<T>> {
public:
    LinkedListIterator(): m_itr(nullptr)
    {}

    explicit LinkedListIterator(ListItem<T> *it): m_itr(it)
    {}

    void swap(LinkedListIterator &other) {
        std::swap(m_itr, other.m_itr);
    }
    
    // pre-increment
    LinkedListIterator operator++() {
        if(m_itr) {
            m_itr = m_itr->next;
        }
    }
    
    // post-increment
    LinkedListIterator operator++(int) {
        LinkedListIterator itr(*this);
        operator++();
        return itr;
    }

    ListItem<T> *operator*() const {
        return m_itr;
    }
    
    ListItem<T> *operator->() const {
        return m_itr;
    }

    bool operator==(const LinkedListIterator &other) const {
        return m_itr == other.m_itr; 
    }
    
    bool operator!=(const LinkedListIterator &other) const {
        return m_itr != other.m_itr; 
    }

private:
    ListItem<T> *m_itr;
};

/*
 * @biref Template class which represents
 * linked list data structure.
 *
 * Every element of list is of type @ListItem.
 * Provides @begin() and @end() and can be
 * iterated like this:
 *
 * @code
 * for(auto &it : myList) {
 *      //do smth with it
 * }
 * @endcode
 */
template <typename T>
class LinkedList {
public:
    LinkedList(): m_head(nullptr),
                  m_tail(nullptr)
    {}

    LinkedList(const LinkedList &other) {
        ListItem<T> *prev{};

        for(const auto &it : other) {
            ListItem<T> *i = new ListItem<T>(it->value);
            if(!m_head) {
                m_head = i;
            }
            i->prev = prev;
            if(i->prev) {
                i->prev->next = i;
            }
            prev = i;
        }
        m_tail = prev;
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

    void remove(const ListItem<T> *i) {
        if(!i)
            return;
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

    void remove(const LinkedListIterator<T> &i) {
        remove(*i);
    }

    void remove(const std::size_t n) {
        const auto &it = find(at(n));
        if(it == end())
            return;
        remove(it);
    }

    LinkedListIterator<T> find(const T &v) {
        if(!m_head)
            return end();
        for(const auto &it : *this) {
            if(it->value == v) {
                return LinkedListIterator<T>(it);
            }
        }
        return end();
    }
    
    std::size_t size() const {
        if(!m_head)
            return 0;
        std::size_t n = 0;
        ListItem<T> *i = m_head;
        do {
            n++;
        } while((i = i->next));
        return n;
    }

    bool isEmpty() const { 
        return size() == 0;
    }

    LinkedListIterator<T> begin() const {
        return LinkedListIterator<T>(m_head);
    }

    LinkedListIterator<T> end() const {
        return LinkedListIterator<T>(nullptr);
    }

    T &at(std::size_t n) const {
        for(const auto &it : *this) {
            if(!n--) {
                return it->value;
            }
        }
    }

    T &operator[](std::size_t n) const {
        return at(n);
    }
    
private:
    ListItem<T> *m_head;
    ListItem<T> *m_tail;

};

// forward declare to use in @Array
template <typename T>
class ArrayIterator;

/*
 * @biref Template class which represents
 * dynamic array.
 *
 * The array grows twice in size when it
 * becames full.
 */
template <typename T>
class Array {
public:
    Array(std::size_t capacity = 16): m_size(0), 
                                 m_capacity(capacity) 
    {
        m_array = new T[m_capacity];
    }

    Array(const Array &other) {
        m_capacity = other.m_capacity;
        m_size = other.m_size;
        m_array = new T[m_capacity];
        std::copy(other.m_array, other.m_array+other.m_size, m_array);
    }

    Array(Array &&other) {
        m_capacity = other.m_capacity;
        m_size = other.m_size;
        m_array = other.m_array;
        other.m_size = 0;
        other.m_capacity = 0;
        other.m_array = nullptr;
    }

    ~Array() {
        if(m_array) {
            delete [] m_array;
        }
    }

    // potentially unsafe
    T &operator[](const std::size_t i) {
        return m_array[i];
    }

    std::size_t size() const {
        return m_size;
    }

    std::size_t capacity() const {
        return m_capacity;
    }

    void clear() {
        m_size = 0;
    }

    const T *data() const {
        return m_array;
    }

    void pushBack(const T &element) {
        if(!m_array) {
            if(!m_capacity) {
                m_capacity = 16;
            }
            m_array = new T[m_capacity];
            m_size = 0;
        }
        m_array[m_size++] = element;
        if(m_size==m_capacity) {
            m_capacity*=2;
            T *newArray = new T[m_capacity];
            std::copy(m_array, m_array+m_size, newArray);
            delete [] m_array;
            m_array = newArray;
        }
    }

    void resize(std::size_t cap) {
        if(cap == 0) {
            if(m_array) {
                delete [] m_array;
            }
            m_array = nullptr;
            m_size = 0;
            m_capacity = 0;
            return;
        }
        T *newArray = new T[cap];
        m_capacity = cap;
        if(m_size>m_capacity) {
            std::copy(m_array, m_array+m_capacity, newArray);
        } else {
            std::copy(m_array, m_array+m_size, newArray);
        }
    }

    ArrayIterator<T> begin() const {
        return ArrayIterator<T>(m_array);
    }

    ArrayIterator<T> end() const {
        return ArrayIterator<T>(m_array+m_size);
    }

    ArrayIterator<T> find(const T &v) const {
        for(auto &it : *this) {
            if(it == v) {
                return ArrayIterator<T>(&it);
            }
        }
        return end();
    }

private:
    T *m_array;
    std::size_t m_size;
    std::size_t m_capacity;
};

/**
 * @biref Iterator for @Array container
 */
template <typename T>
class ArrayIterator : public std::iterator<std::forward_iterator_tag, T> {
public:
    ArrayIterator(): m_itr(nullptr)
    {}

    explicit ArrayIterator(T *data): m_itr(data)
    {}

    void swap(ArrayIterator &other) {
        std::swap(m_itr, other.m_itr);
    }
    
    // pre-increment
    ArrayIterator operator++() {
        m_itr++;
    }
    
    // post-increment
    ArrayIterator operator++(int) {
        ArrayIterator itr(*this);
        m_itr++;
        return itr;
    }

    T &operator*() const {
        return *m_itr;
    }
    
    T &operator->() const {
        return *m_itr;
    }

    bool operator==(const ArrayIterator &other) const {
        return m_itr == other.m_itr; 
    }
    
    bool operator!=(const ArrayIterator &other) const {
        return m_itr != other.m_itr; 
    }

private:
    T *m_itr;
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

/**
 * @brief Hash function for Array of bytes
 *
 * Computes BSD checksum of byte array.
 */
template<>
std::size_t hash(const Array<std::uint8_t> &arr) {
    std::size_t checksum = 0;
    for(const auto &it : arr) {
        checksum = (checksum >> 1) + ((checksum & 1) << 32);
        checksum+=it;
    }
    return checksum;
}

/**
 * @brief A holder for
 * key-value pairs in buckets.
 */
template <typename K, typename V>
struct KeyVal {
    KeyVal(K k = K{}, const V v = V{}): key(k), value(v)
    {}
    K key;
    V value;
};

/**
 * @brief Iterator for HashMap
 * Linearely iterates over all key-value pairs
 * of all buckets.
 */
template <typename K, typename V>
class HashMapIterator: public std::iterator<std::forward_iterator_tag, KeyVal<K, V>> {
public:
    HashMapIterator()
    {}

    explicit HashMapIterator(ArrayIterator<LinkedList<KeyVal<K, V>>> storageItr,
                            ArrayIterator<LinkedList<KeyVal<K, V>>> storageEnd):
        m_storageItr(storageItr), 
        m_itr((*storageItr).begin()), 
        m_storageEnd(storageEnd)
    {}

    void swap(HashMapIterator &other) {
        std::swap(m_storageItr, other.m_storageItr);
        std::swap(m_itr, other.m_itr);
    }
    
    // pre-increment
    KeyVal<K, V> &operator++() {
        m_itr++;
        auto endOfBucket = (*m_storageItr).end();
        if(m_itr == endOfBucket) {
            if(++m_storageItr != m_storageEnd) {
                m_itr = (*m_storageItr).begin();
            }
        }
        return this->operator*();
    }
    
    // post-increment
    KeyVal<K, V> &operator++(int) {
        HashMapIterator itr(*this);
        m_itr++;
        return itr->value;
    }

    KeyVal<K, V> &operator*() const {
        return m_itr->value;
    }
    
    KeyVal<K, V> &operator->() const {
        return m_itr->value;
    }

    bool operator==(const HashMapIterator &other) const {
        return m_itr == other.m_itr; 
    }
    
    bool operator!=(const HashMapIterator &other) const {
        return m_itr != other.m_itr; 
    }

private:
    ArrayIterator<LinkedList<KeyVal<K, V>>>  m_storageItr;
    ArrayIterator<LinkedList<KeyVal<K, V>>>  m_storageEnd;
    LinkedListIterator<KeyVal<K, V>> m_itr;
};


/*
 * @biref Template class which represents
 * hash map (table) - a generic key-value 
 * container
 *
 * TODO: implement Iterator interface for @HashMap
 */
template <typename K, typename V>
class HashMap {
public:

    /*
     * @brief HashMap constructor
     *
     * @param capacity Specifies initial number of buckets.
     * @param loadFactor Specifies a factor treshhold,
     * which can be computed like so: size/capacity.
     * When it reaches the treshhold, the whole hash map
     * is rehashed: the bucket size is increased by two
     * and hashes are recalculated.
     */
    HashMap(std::size_t capacity = 16,
            float loadFactor = 0.75f ):
            m_loadFactor(loadFactor),
            m_buckets(capacity)
    {
        initBuckets(capacity);
    }
                                        
    HashMap(const HashMap &other):
            m_buckets(other.m_buckets),
            m_loadFactor(other.m_loadFactor)
    {}

    HashMap(HashMap &&other):
            m_buckets(other.m_buckets),
            m_loadFactor(other.m_loadFactor)
    {
        other.m_buckets.clear(); 
    }

    V &get(const K &k) {
        std::size_t h = hash(k)%capacity();
        auto &bucket = m_buckets[h];
        for(const auto &it : bucket) {
            if(it->value.key == k) {
                return it->value.value;
            }
        }
        bucket.pushBack(KeyVal<K, V>(k));
        if(float(size())/capacity()>=m_loadFactor) {
            rehash();
            return get(k);
        } else {
            return bucket[bucket.size()-1].value;
        }
    }

    V &operator[](const K &k) {
        return get(k);
    }

    std::size_t size() const {
        std::size_t n = 0;
        for(const auto &it : m_buckets) {
            n+=it.size();
        }
        return n;
    }

    std::size_t capacity() const {
        return m_buckets.size();
    }

    HashMapIterator<K, V> begin() const {
        return HashMapIterator<K, V>(m_buckets.begin(),
                                     m_buckets.end());
    }

    HashMapIterator<K, V> end() const {
        return HashMapIterator<K, V>(m_buckets.end(),
                                     m_buckets.end());
    }

private:

    void initBuckets(std::size_t n) {
        while(n--) {
            m_buckets.pushBack(LinkedList<KeyVal<K, V>>());
        }
    }

    /**
     * @brief Increases bucket count and
     * performs complete rehashing of data
     */
    void rehash() {
        if(size() == 0) {
            return;
        }

        // temporary std::map-like storage
        Array<KeyVal<K, V>> tempMap(size());
        std::cerr << size() << std::endl;
        for(auto &it : m_buckets) {
            for(const auto &jt : it) {
                tempMap.pushBack(jt->value);
            }
            it.clear();
        }
        std::size_t cap = capacity()*2;
        m_buckets.clear();
        m_buckets.resize(cap);
        initBuckets(cap);

        for(int i = 0;i<tempMap.size();i++) {
            std::size_t h = hash<K>(tempMap[i].key)%capacity();
            auto &bucket = m_buckets[h];
            bucket.pushBack(KeyVal<K, V>(tempMap[i].key, tempMap[i].value));
        }

    }

private:
    Array<LinkedList<KeyVal<K, V>>> m_buckets;
    float m_loadFactor;
};


#endif // HASH_MAP
