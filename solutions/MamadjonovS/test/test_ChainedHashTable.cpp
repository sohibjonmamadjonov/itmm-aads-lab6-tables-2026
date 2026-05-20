#include <gtest.h>
#include "ChainHashTable.h"
 
 
class ChainedHashTableTest : public ::testing::Test {
protected:
    void SetUp() override {
        table = std::make_unique<ChainedHashTable<int, std::string>>(10);
    }

    std::unique_ptr<ChainedHashTable<int, std::string>> table;
};

TEST_F(ChainedHashTableTest, InsertAndSearch) {
    EXPECT_TRUE(table->insert(1, "one"));
    EXPECT_TRUE(table->insert(2, "two"));
    EXPECT_TRUE(table->insert(3, "three"));

    std::string value;
    EXPECT_TRUE(table->search(2, value));
    EXPECT_EQ(value, "two");
    EXPECT_EQ(table->size(), 3);
}

TEST_F(ChainedHashTableTest, InsertDuplicateKey) {
    EXPECT_TRUE(table->insert(1, "one"));
    EXPECT_FALSE(table->insert(1, "one_duplicate"));
    EXPECT_EQ(table->size(), 1);
}

TEST_F(ChainedHashTableTest, RemoveExistingKey) {
    table->insert(1, "one");
    table->insert(2, "two");

    EXPECT_TRUE(table->remove(1));
    EXPECT_EQ(table->size(), 1);
    EXPECT_FALSE(table->contains(1));
    EXPECT_TRUE(table->contains(2));
}

TEST_F(ChainedHashTableTest, RemoveNonExistingKey) {
    EXPECT_FALSE(table->remove(999));
    EXPECT_EQ(table->size(), 0);
}

TEST_F(ChainedHashTableTest, Contains) {
    table->insert(5, "five");
    EXPECT_TRUE(table->contains(5));
    EXPECT_FALSE(table->contains(10));
}

TEST_F(ChainedHashTableTest, ClearAndEmpty) {
    table->insert(1, "one");
    table->insert(2, "two");
    EXPECT_FALSE(table->empty());

    table->clear();
    EXPECT_TRUE(table->empty());
    EXPECT_EQ(table->size(), 0);
    EXPECT_FALSE(table->contains(1));
}

TEST_F(ChainedHashTableTest, OperationCount) {
    table->insert(1, "one");
    size_t count = table->getOperationCount();
    EXPECT_GT(count, 0);

    table->resetOperationCount();
    EXPECT_EQ(table->getOperationCount(), 0);
}