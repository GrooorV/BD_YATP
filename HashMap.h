#pragma once

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

    void rehash() {
        unsigned int oldCapacity = capacity;
        Entry* oldTable = table;

        capacity = capacity * 2;
        table = new Entry[capacity];
        count = 0;

        for (unsigned int i = 0; i < oldCapacity; i++) {
            if (oldTable[i].occupied) {
                insert(oldTable[i].key, oldTable[i].value);
            }
        }

        delete[] oldTable;
    }

public:
    HashMap() : capacity(100), count(0) {
        table = new Entry[capacity];
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
        unsigned int indx = hash(key);
        unsigned int startIndex = indx;
        while (table[indx].occupied) {
            if (table[indx].key == key) {
                table[indx].occupied = false;
                count--;
                return true;
            }
            indx = (indx + 1) % capacity;
            if (indx == startIndex) break;
        }
        return false;
    }

    V* find(const K& key) {
        unsigned int indx = hash(key);
        unsigned int startIndex = indx;
        while (table[indx].occupied) {
            if (table[indx].key == key) {
                return &table[indx].value;
            }
            indx = (indx + 1) % capacity;
            if (indx == startIndex) break;
        }
        return nullptr;
    }

    const V* find(const K& key) const {
        unsigned int indx = hash(key);
        unsigned int startIndex = indx;
        while (table[indx].occupied) {
            if (table[indx].key == key) {
                return &table[indx].value;
            }
            indx = (indx + 1) % capacity;
            if (indx == startIndex) break;
        }
        return nullptr;
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
