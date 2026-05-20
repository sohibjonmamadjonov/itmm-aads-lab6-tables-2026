#include <gtest.h>
#include "OpenAddressingHashTable.h"

class OpenAddressingHashTableTest : public ::testing::Test {
protected:
    void SetUp() override {
        table = std::make_unique<OpenAddressingHashTable<int, std::string>>(20);
    }

    std::unique_ptr<OpenAddressingHashTable<int, std::string>> table;
};

TEST_F(OpenAddressingHashTableTest, InsertAndSearch) {
    EXPECT_TRUE(table->insert(10, "ten"));
    EXPECT_TRUE(table->insert(20, "twenty"));
    EXPECT_TRUE(table->insert(30, "thirty"));

    std::string value;
    EXPECT_TRUE(table->search(20, value));
    EXPECT_EQ(value, "twenty");
    EXPECT_EQ(table->size(), 3);
}

TEST_F(OpenAddressingHashTableTest, InsertDuplicateKey) {
    EXPECT_TRUE(table->insert(5, "five"));
    EXPECT_FALSE(table->insert(5, "five_again"));
    EXPECT_EQ(table->size(), 1);
}

TEST_F(OpenAddressingHashTableTest, InsertWhenAlmostFull) {
    for (int i = 0; i < 14; i++) {  // 70% of 20
        EXPECT_TRUE(table->insert(i, std::to_string(i)));
    }
    // Should fail when exceeding 70% load factor
    EXPECT_FALSE(table->insert(100, "overflow"));
}

TEST_F(OpenAddressingHashTableTest, RemoveAndSearch) {
    table->insert(1, "one");
    table->insert(2, "two");

    EXPECT_TRUE(table->remove(1));
    EXPECT_EQ(table->size(), 1);

    std::string value;
    EXPECT_FALSE(table->search(1, value));
    EXPECT_TRUE(table->search(2, value));
    EXPECT_EQ(value, "two");
}

TEST_F(OpenAddressingHashTableTest, RemoveNonExistingKey) {
    EXPECT_FALSE(table->remove(999));
    EXPECT_EQ(table->size(), 0);
}

TEST_F(OpenAddressingHashTableTest, ProbingSequence) {
    // Insert keys that might cause collisions
    table->insert(5, "five");
    table->insert(25, "twenty_five");  // Same hash as 5 if size=20
    table->insert(45, "forty_five");

    std::string value;
    EXPECT_TRUE(table->search(25, value));
    EXPECT_EQ(value, "twenty_five");
    EXPECT_EQ(table->size(), 3);
}

TEST_F(OpenAddressingHashTableTest, ClearAndEmpty) {
    table->insert(1, "one");
    table->insert(2, "two");
    EXPECT_FALSE(table->empty());

    table->clear();
    EXPECT_TRUE(table->empty());
    EXPECT_EQ(table->size(), 0);
}

TEST_F(OpenAddressingHashTableTest, DeletedMarkers) {
    table->insert(1, "one");
    table->insert(2, "two");
    table->remove(1);

    // Should be able to insert new key at deleted position
    EXPECT_TRUE(table->insert(3, "three"));
    EXPECT_EQ(table->size(), 2);
}