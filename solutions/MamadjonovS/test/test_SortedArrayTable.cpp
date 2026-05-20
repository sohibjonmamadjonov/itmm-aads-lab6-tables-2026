 
#include <gtest.h>
#include "SortedArrayTable.h"

class SortedArrayTableTest : public ::testing::Test {
protected:
    void SetUp() override {
        table = std::make_unique<SortedArrayTable<int, std::string>>();
    }

    std::unique_ptr<SortedArrayTable<int, std::string>> table;
};

TEST_F(SortedArrayTableTest, InsertAndMaintainOrder) {
    EXPECT_TRUE(table->insert(3, "three"));
    EXPECT_TRUE(table->insert(1, "one"));
    EXPECT_TRUE(table->insert(2, "two"));

    std::string value;
    EXPECT_TRUE(table->search(1, value));
    EXPECT_EQ(value, "one");
    EXPECT_TRUE(table->search(2, value));
    EXPECT_EQ(value, "two");
    EXPECT_TRUE(table->search(3, value));
    EXPECT_EQ(value, "three");
    EXPECT_EQ(table->size(), 3);
}

TEST_F(SortedArrayTableTest, InsertDuplicateKey) {
    EXPECT_TRUE(table->insert(5, "five"));
    EXPECT_FALSE(table->insert(5, "five_duplicate"));
    EXPECT_EQ(table->size(), 1);
}

TEST_F(SortedArrayTableTest, RemoveFromBeginning) {
    table->insert(10, "ten");
    table->insert(20, "twenty");
    table->insert(30, "thirty");

    EXPECT_TRUE(table->remove(10));
    EXPECT_EQ(table->size(), 2);
    EXPECT_FALSE(table->contains(10));
    EXPECT_TRUE(table->contains(20));
    EXPECT_TRUE(table->contains(30));
}

TEST_F(SortedArrayTableTest, ClearAndEmpty) {
    table->insert(1, "one");
    table->insert(2, "two");
    EXPECT_FALSE(table->empty());

    table->clear();
    EXPECT_TRUE(table->empty());
    EXPECT_EQ(table->size(), 0);
}

TEST_F(SortedArrayTableTest, OperationCount) {
    table->insert(1, "one");
    int count = table->getOperationCount();
    EXPECT_GT(count, 0);

    table->resetOperationCount();
    EXPECT_EQ(table->getOperationCount(), 0);
}