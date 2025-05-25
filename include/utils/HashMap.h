#pragma once
#include <functional>
#include "DynamicArray.h"


template<typename K, typename V>
class HashMap {
private:
    struct Entry {
        K key;
        V value;
        bool occupied;

        Entry() : occupied(false) {}
    };

    Entry* table;
    unsigned int capacity;
    unsigned int count;

    unsigned int hash(const K& key) const {
        return std::hash<K>{}(key) % capacity;
    }

    void insertWithoutRehash(const K& key, const V& value) {
        unsigned int idx = hash(key);
        while (table[idx].occupied) {
            idx = (idx + 1) % capacity;
        }
        table[idx].key = key;
        table[idx].value = value;
        table[idx].occupied = true;
        count++;
    }

    void rehash() {
        unsigned int oldCapacity = capacity;
        Entry* oldTable = table;

        capacity *= 2;
        table = new Entry[capacity];
        count = 0;

        for (unsigned int i = 0; i < oldCapacity; i++) {
            if (oldTable[i].occupied) {
                insertWithoutRehash(oldTable[i].key, oldTable[i].value);
            }
        }

        delete[] oldTable;
    }

public:
    HashMap() : capacity(8), count(0) {
        table = new Entry[capacity]{};
    }

    ~HashMap() {
        delete[] table;
    }

    void insert(const K& key, const V& value) {
        if (count * 2 >= capacity) {
            rehash();
        }

        unsigned int idx = hash(key);
        while (table[idx].occupied) {
            if (table[idx].key == key) {
                table[idx].value = value;
                return;
            }
            idx = (idx + 1) % capacity;
        }

        table[idx].key = key;
        table[idx].value = value;
        table[idx].occupied = true;
        count++;
    }

    bool erase(const K& key) {
        unsigned int idx = hash(key);
        unsigned int startIndex = idx;
        while (table[idx].occupied) {
            if (table[idx].key == key) {
                table[idx].occupied = false;
                count--;
                return true;
            }
            idx = (idx + 1) % capacity;
            if (idx == startIndex) break;
        }
        return false;
    }

    V find(const K& key) const {
        unsigned int idx = hash(key);
        unsigned int startIndex = idx;
        while (table[idx].occupied) {
            if (table[idx].key == key) {
                return table[idx].value;
            }
            idx = (idx + 1) % capacity;
            if (idx == startIndex) break;
        }
        return V{};  // безопасный дефолт
    }

    DynamicArray<K> getKeys() const {
        DynamicArray<K> keys;
        for (unsigned int i = 0; i < capacity; ++i) {
            if (table[i].occupied) {
                keys.append(table[i].key);
            }
        }
        return keys;
    }

    unsigned int size() const {
        return count;
    }

    bool isEmpty() const {
        return count == 0;
    }

    void clear() {
        delete[] table;
        capacity = 8;
        count = 0;
        table = new Entry[capacity];
    }
};