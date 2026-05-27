#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <chrono>

template<typename K, typename V>
class Table {
public:
    virtual ~Table() = default;

    // Основные операции
    virtual bool insert(const K& key, const V& value) = 0;
    virtual bool remove(const K& key) = 0;
    virtual bool search(const K& key, V& value) = 0;
    virtual bool contains(const K& key) = 0;
    virtual size_t size() const = 0;
    virtual bool empty() const = 0;
    virtual void clear() = 0;
    virtual std::string getName() const = 0;

    // Для тестирования
    int getOperationCount() const { return operationCount; }
    void resetOperationCount() { operationCount = 0; }
 

protected:  
    void incOperationCount() { operationCount++; }

private:
    int operationCount = 0;
};

#endif