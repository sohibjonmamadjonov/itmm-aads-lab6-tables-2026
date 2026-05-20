#ifndef CHAIN_HASH_TABLE_H
#define CHAIN_HASH_TABLE_H

#include "table.h"
#include <list>
#include <vector>
#include <string>
#include <cmath>

template<typename K, typename V>
class ChainedHashTable : public Table<K, V> {
private:
    struct Entry {
        K key;
        V value;
        Entry(const K& k, const V& v) : key(k), value(v) {}
    };

    std::vector<std::list<Entry>> table;
    size_t tableSize;
    size_t elementCount;

    size_t hash(const K& key) const {
        double fractional = std::fmod(static_cast<double>(key) * 0.6180339887498949, 1.0);
        return static_cast<size_t>(fractional * tableSize);
    }

public:
    ChainedHashTable(size_t size = 1000) : tableSize(size), elementCount(0) {
        table.resize(size);
    }

    bool insert(const K& key, const V& value) override {
        this->resetOperationCount();
        size_t idx = hash(key);

        for (const auto& entry : table[idx]) {
            this->incOperationCount();
            if (entry.key == key) return false;
        }

        table[idx].emplace_back(key, value);
        elementCount++;
        this->incOperationCount();
        return true;
    }

    bool remove(const K& key) override {
        this->resetOperationCount();
        size_t idx = hash(key);

        auto& bucket = table[idx];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            this->incOperationCount();
            if (it->key == key) {
                bucket.erase(it);
                elementCount--;
                return true;
            }
        }
        return false;
    }

    bool search(const K& key, V& value) override {
        this->resetOperationCount();
        size_t idx = hash(key);

        for (const auto& entry : table[idx]) {
            this->incOperationCount();
            if (entry.key == key) {
                value = entry.value;
                return true;
            }
        }
        return false;
    }

    bool contains(const K& key) override {
        this->resetOperationCount();
        size_t idx = hash(key);
        for (const auto& entry : table[idx]) {
            this->incOperationCount();
            if (entry.key == key) return true;
        }
        return false;
    }

    size_t size() const override { return elementCount; }
    bool empty() const override { return elementCount == 0; }
    void clear() override {
        for (auto& bucket : table) bucket.clear();
        elementCount = 0;
    }

    std::string getName() const override { return "Chained Hash Table"; }
};

#endif