#include <gtest.h>
#include "AVLTreeTable.h"

class AVLTreeTableTest : public ::testing::Test {
protected:
    void SetUp() override {
        tree = std::make_unique<AVLTreeTable<int, std::string>>();
    }

    std::unique_ptr<AVLTreeTable<int, std::string>> tree;
};

TEST_F(AVLTreeTableTest, InsertAndSearch) {
    EXPECT_TRUE(tree->insert(50, "fifty"));
    EXPECT_TRUE(tree->insert(30, "thirty"));
    EXPECT_TRUE(tree->insert(70, "seventy"));
    EXPECT_TRUE(tree->insert(20, "twenty"));
    EXPECT_TRUE(tree->insert(40, "forty"));

    std::string value;
    EXPECT_TRUE(tree->search(30, value));
    EXPECT_EQ(value, "thirty");
    EXPECT_TRUE(tree->search(70, value));
    EXPECT_EQ(value, "seventy");
    EXPECT_EQ(tree->size(), 5);  // size() returns 0 in implementation
}

TEST_F(AVLTreeTableTest, InsertDuplicateKey) {
    EXPECT_TRUE(tree->insert(10, "ten"));
    EXPECT_FALSE(tree->insert(10, "ten_again"));
}

TEST_F(AVLTreeTableTest, RemoveLeafNode) {
    tree->insert(50, "fifty");
    tree->insert(30, "thirty");
    tree->insert(70, "seventy");

    EXPECT_TRUE(tree->remove(30));
    EXPECT_FALSE(tree->contains(30));
    EXPECT_TRUE(tree->contains(50));
    EXPECT_TRUE(tree->contains(70));
}

TEST_F(AVLTreeTableTest, RemoveNodeWithOneChild) {
    tree->insert(50, "fifty");
    tree->insert(30, "thirty");
    tree->insert(20, "twenty");

    EXPECT_TRUE(tree->remove(30));
    EXPECT_FALSE(tree->contains(30));
    EXPECT_TRUE(tree->contains(20));
}

TEST_F(AVLTreeTableTest, RemoveNodeWithTwoChildren) {
    tree->insert(50, "fifty");
    tree->insert(30, "thirty");
    tree->insert(70, "seventy");
    tree->insert(60, "sixty");
    tree->insert(80, "eighty");

    EXPECT_TRUE(tree->remove(70));
    EXPECT_FALSE(tree->contains(70));
    EXPECT_TRUE(tree->contains(60));
    EXPECT_TRUE(tree->contains(80));
}

TEST_F(AVLTreeTableTest, RemoveNonExistingKey) {
    EXPECT_FALSE(tree->remove(999));
}

TEST_F(AVLTreeTableTest, Contains) {
    tree->insert(100, "hundred");
    EXPECT_TRUE(tree->contains(100));
    EXPECT_FALSE(tree->contains(200));
}

TEST_F(AVLTreeTableTest, BalanceAfterInserts) {
    // Insert in order that would cause imbalance
    tree->insert(10, "ten");
    tree->insert(20, "twenty");
    tree->insert(30, "thirty");
    tree->insert(40, "forty");
    tree->insert(50, "fifty");

    // All should still be accessible
    EXPECT_TRUE(tree->contains(10));
    EXPECT_TRUE(tree->contains(20));
    EXPECT_TRUE(tree->contains(30));
    EXPECT_TRUE(tree->contains(40));
    EXPECT_TRUE(tree->contains(50));
}

TEST_F(AVLTreeTableTest, ClearAndEmpty) {
    tree->insert(1, "one");
    tree->insert(2, "two");
    EXPECT_FALSE(tree->empty());

    tree->clear();
    EXPECT_TRUE(tree->empty());
    EXPECT_FALSE(tree->contains(1));
}