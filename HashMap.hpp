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

/**
 * @biref A generic hash function
 *
 * Used by @HashMap.
 * Template specifications
 * should be provided for
 * for every required type.
 */
template <typename T>
std::size_t hash(const T &o);

/**
 * @biref Template class which represents 
 * single element of linked list.
 *
 * Also is used as iterator of @LinkedList
 */
template <typename T>
struct ListItem {
    ListItem(const T &v): value(v),
                          prev(nullptr), 
                          next(nullptr)
    {}

    const ListItem<T> *operator++() const {
        return next;
    }

    bool operator==(const T &v) const {
        return value == v;
    }

    T value;
    ListItem *prev;
    ListItem *next;
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

        for(auto &it : other) {
            ListItem<T> *i = new ListItem<T>(it.value);
            if(!m_head) {
                m_head = i;
            }
            i->prev = prev;
            if(i->prev) {
                i->prev->next = i;
            }
        }
    }
    
    LinkedList(LinkedList &&other): m_head(other.m_head),
                                    m_tail(other.m_tail)                                 
    {
        other.m_head = nullptr;
        other.m_tail = nullptr;
    }

    ~LinkedList() {
        if(!m_head) {
            return;
        }
        ListItem<T> *i = m_head;
        ListItem<T> *tmp{};
        do {
            tmp = i->next;
            delete i;
            i = tmp;
        } while(i);
    }

    void pushBack(const T &val) {
        if(!m_tail) {
            m_head = new ListItem<T>(val);
            m_tail = m_head;
        } else {
            m_tail->next = new ListItem<T>(val);  
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

    const ListItem<T> *find(const T &v) {
        if(!m_head)
            return nullptr;
        for(auto it : m_head) {
            if(it == v)
                return it;
        }
        return nullptr;
    }
    
    std::size_t size() const {
        if(!m_head)
            return 0;
        std::size_t n = 0;
        for(auto it : m_head) {
            n++;
        }
        return n;
    }

    bool isEmpty() const { 
        return size() == 0;
    }

    const ListItem<T> *begin() const {
        return m_head;
    }

    const ListItem<T> *end() const {
        return m_tail;
    }
    
private:
    ListItem<T> *m_head;
    ListItem<T> *m_tail;

};

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
        std::copy(m_array, other.m_array, m_size);
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

    void pushBack(const T &element) {
        m_array[m_size++] = element;
        if(m_size>m_capacity) {
            m_capacity*=2;
            T *newArray = new T[m_capacity];
            std::copy(newArray, m_array, m_size);
            delete [] m_array;
            m_array = newArray;
        }
    }

private:
    T *m_array;
    std::size_t m_size;
    std::size_t m_capacity;
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
    {}
                                        
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

    V &operator[](const K &k) {
        
    }

    std::size_t size() const {
        std::size_t n = 0;
        // TODO: implement Iterator interface for @Array
        for(int i = 0;i<m_buckets.size();i++) {
            n+=m_buckets[i].size();
        }
        return n;
    }

private:
    void rehash();

private:
    Array<LinkedList<K>> m_buckets;
    float m_loadFactor;
};

#endif // HASH_MAP
