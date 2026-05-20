#ifndef SORTED_ARRAY_TABLE_H
#define SORTED_ARRAY_TABLE_H

#include "table.h"
#include <algorithm>
#include <vector>
#include <string>

template<typename K, typename V>
class SortedArrayTable : public Table<K, V> {
private:
    struct Entry {
        K key;
        V value;
        Entry(const K& k, const V& v) : key(k), value(v) {}
        bool operator<(const Entry& other) const { return key < other.key; }
        bool operator<(const K& k) const { return key < k; }
    };

    std::vector<Entry> data;

    int binarySearch(const K& key) {
        int left = 0, right = static_cast<int>(data.size()) - 1;
        while (left <= right) {
            this->incOperationCount();
            int mid = left + (right - left) / 2;
            if (data[mid].key == key) return mid;
            if (data[mid].key < key) left = mid + 1;
            else right = mid - 1;
        }
        return -1;
    }

public:
    bool insert(const K& key, const V& value) override {
        this->resetOperationCount();

        int left = 0, right = static_cast<int>(data.size()) - 1;

        while (left <= right) {
            this->incOperationCount();
            int mid = left + (right - left) / 2;
            if (data[mid].key == key) return false;
            if (data[mid].key < key) left = mid + 1;
            else right = mid - 1;
        }
        int pos = left;

        data.insert(data.begin() + pos, Entry(key, value));
        for (size_t i = 0; i < data.size() - pos; ++i) {
            this->incOperationCount();
        }
        return true;
    }

    bool remove(const K& key) override {
        this->resetOperationCount();
        int idx = binarySearch(key);
        if (idx == -1) return false;

        data.erase(data.begin() + idx);
        for (size_t i = 0; i < data.size() - idx; ++i) {
            this->incOperationCount();
        }
        return true;
    }

    bool search(const K& key, V& value) override {
        this->resetOperationCount();
        int idx = binarySearch(key);
        if (idx != -1) {
            value = data[idx].value;
            return true;
        }
        return false;
    }

    bool contains(const K& key) override {
        this->resetOperationCount();
        return binarySearch(key) != -1;
    }

    size_t size() const override { return data.size(); }
    bool empty() const override { return data.empty(); }
    void clear() override { data.clear(); }

    std::string getName() const override { return "Sorted Array Table"; }
};

#endif