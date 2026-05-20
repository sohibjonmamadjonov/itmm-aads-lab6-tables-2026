#ifndef OPEN_ADDRESSING_HASH_TABLE_H
#define OPEN_ADDRESSING_HASH_TABLE_H

#include "table.h"
#include <vector>
#include <string>
#include <cmath>

template<typename K, typename V>
class OpenAddressingHashTable : public Table<K, V> {
private:
    struct Entry {
        K key;
        V value;
        bool occupied;
        bool deleted;

        Entry() : occupied(false), deleted(false) {}
        Entry(const K& k, const V& v) : key(k), value(v), occupied(true), deleted(false) {}
    };

    std::vector<Entry> table;
    size_t tableSize;
    size_t elementCount;

    size_t hash(const K& key) const {
        double fractional = std::fmod(static_cast<double>(key) * 0.6180339887498949, 1.0);
        return static_cast<size_t>(fractional * tableSize);
    }

    size_t probe(size_t h, size_t i) const {
        return (h + i) % tableSize;
    }

public:
    OpenAddressingHashTable(size_t size = 2000) : tableSize(size), elementCount(0) {
        table.resize(size);
    }

    bool insert(const K& key, const V& value) override {
        this->resetOperationCount();
        if (elementCount >= tableSize * 0.7) return false;

        size_t h = hash(key);

        for (size_t i = 0; i < tableSize; ++i) {
            this->incOperationCount();
            size_t idx = probe(h, i);

            if (!table[idx].occupied || table[idx].deleted) {
                table[idx] = Entry(key, value);
                elementCount++;
                return true;
            }

            if (table[idx].occupied && !table[idx].deleted && table[idx].key == key) {
                return false;
            }
        }
        return false;
    }

    bool remove(const K& key) override {
        this->resetOperationCount();
        size_t h = hash(key);

        for (size_t i = 0; i < tableSize; ++i) {
            this->incOperationCount();
            size_t idx = probe(h, i);

            if (!table[idx].occupied) return false;

            if (table[idx].occupied && !table[idx].deleted && table[idx].key == key) {
                table[idx].deleted = true;
                table[idx].occupied = false;
                elementCount--;
                return true;
            }
        }
        return false;
    }

    bool search(const K& key, V& value) override {
        this->resetOperationCount();
        size_t h = hash(key);

        for (size_t i = 0; i < tableSize; ++i) {
            this->incOperationCount();
            size_t idx = probe(h, i);

            if (!table[idx].occupied) return false;

            if (table[idx].occupied && !table[idx].deleted && table[idx].key == key) {
                value = table[idx].value;
                return true;
            }
        }
        return false;
    }

    bool contains(const K& key) override {
        this->resetOperationCount();
        V dummy;
        return search(key, dummy);
    }

    size_t size() const override { return elementCount; }
    bool empty() const override { return elementCount == 0; }
    void clear() override {
        for (auto& entry : table) {
            entry.occupied = false;
            entry.deleted = false;
        }
        elementCount = 0;
    }

    std::string getName() const override { return "Open Addressing Hash Table"; }
};

#endif