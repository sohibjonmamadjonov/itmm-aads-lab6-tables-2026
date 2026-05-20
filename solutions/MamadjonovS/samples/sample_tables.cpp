#include <iostream>
#include <vector>
#include <memory>

#include "table.h"
#include "SortedArrayTable.h"
#include "AVLTreeTable.h"
#include "ChainHashTable.h"
#include "OpenAddressingHashTable.h"

int main()
{
    std::vector<Table<int, int>*> tables;

    auto* linear_table = new SortedArrayTable<int, int>();
    auto* search_tree = new AVLTreeTable<int, int>();
    auto* hash_chain = new ChainedHashTable<int, int>();
    auto* hash_open = new OpenAddressingHashTable<int, int>();

    tables.push_back(linear_table);
    tables.push_back(search_tree);
    tables.push_back(hash_chain);
    tables.push_back(hash_open);

    for (auto* table : tables)
    {
        table->resetOperationCount();
        table->insert(5, 55);
        int value;
        if (table->search(5, value))
            std::cout << value << "\n";
        else
            std::cout << "0\n";
        table->remove(5);
        if (table->search(5, value))
            std::cout << value << "\n";
        else
            std::cout << "0\n";
    }

    // Clean up
    for (auto* table : tables) {
        delete table;
    }

    return 0;
}