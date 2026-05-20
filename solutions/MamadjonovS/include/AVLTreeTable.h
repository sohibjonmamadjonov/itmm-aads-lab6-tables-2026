#ifndef AVL_TREE_TABLE_H
#define AVL_TREE_TABLE_H

#include "table.h"
#include <memory>
#include <string>
#include <algorithm>

template<typename K, typename V>
class AVLTreeTable : public Table<K, V> {
private:
    struct Node {
        K key;
        V value;
        int height;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        Node(const K& k, const V& v) : key(k), value(v), height(1), left(nullptr), right(nullptr) {}
    };

    std::unique_ptr<Node> root;
    size_t nodeCount;

    int height(Node* node) const {
        return node ? node->height : 0;
    }

    int balanceFactor(Node* node) const {
        return node ? height(node->left.get()) - height(node->right.get()) : 0;
    }

    void updateHeight(Node* node) {
        if (node) {
            node->height = 1 + std::max(height(node->left.get()), height(node->right.get()));
        }
    }

    std::unique_ptr<Node> rotateRight(std::unique_ptr<Node> y) {
        this->incOperationCount();
        auto x = std::move(y->left);
        y->left = std::move(x->right);
        x->right = std::move(y);
        updateHeight(x->right.get());
        updateHeight(x.get());
        return x;
    }

    std::unique_ptr<Node> rotateLeft(std::unique_ptr<Node> x) {
        this->incOperationCount();
        auto y = std::move(x->right);
        x->right = std::move(y->left);
        y->left = std::move(x);
        updateHeight(y->left.get());
        updateHeight(y.get());
        return y;
    }

    std::unique_ptr<Node> balance(std::unique_ptr<Node> node) {
        if (!node) return node;

        updateHeight(node.get());
        int bf = balanceFactor(node.get());

        if (bf > 1 && balanceFactor(node->left.get()) >= 0) {
            return rotateRight(std::move(node));
        }
        if (bf > 1 && balanceFactor(node->left.get()) < 0) {
            node->left = rotateLeft(std::move(node->left));
            return rotateRight(std::move(node));
        }
        if (bf < -1 && balanceFactor(node->right.get()) <= 0) {
            return rotateLeft(std::move(node));
        }
        if (bf < -1 && balanceFactor(node->right.get()) > 0) {
            node->right = rotateRight(std::move(node->right));
            return rotateLeft(std::move(node));
        }

        return node;
    }

    std::unique_ptr<Node> insert(std::unique_ptr<Node> node, const K& key, const V& value) {
        if (!node) {
            this->incOperationCount();
            nodeCount++;
            return std::make_unique<Node>(key, value);
        }

        this->incOperationCount();
        if (key < node->key) {
            node->left = insert(std::move(node->left), key, value);
        }
        else if (key > node->key) {
            node->right = insert(std::move(node->right), key, value);
        }
        else {
            return node;
        }

        return balance(std::move(node));
    }

    Node* search(Node* node, const K& key) {
        if (!node) return nullptr;
        this->incOperationCount();
        if (key == node->key) return node;
        if (key < node->key) return search(node->left.get(), key);
        return search(node->right.get(), key);
    }

    std::unique_ptr<Node> remove(std::unique_ptr<Node> node, const K& key) {
        if (!node) return nullptr;

        this->incOperationCount();
        if (key < node->key) {
            node->left = remove(std::move(node->left), key);
        }
        else if (key > node->key) {
            node->right = remove(std::move(node->right), key);
        }
        else {
            if (!node->left || !node->right) {
                nodeCount--;
                return (node->left) ? std::move(node->left) : std::move(node->right);
            }

            Node* minNode = node->right.get();
            while (minNode->left) minNode = minNode->left.get();
            node->key = minNode->key;
            node->value = minNode->value;
            node->right = remove(std::move(node->right), minNode->key);
        }

        return balance(std::move(node));
    }

public:
    AVLTreeTable() : root(nullptr), nodeCount(0) {}

    bool insert(const K& key, const V& value) override {
        this->resetOperationCount();
        if (contains(key)) return false;
        root = insert(std::move(root), key, value);
        return true;
    }

    bool remove(const K& key) override {
        this->resetOperationCount();
        if (!contains(key)) return false;
        root = remove(std::move(root), key);
        return true;
    }

    bool search(const K& key, V& value) override {
        this->resetOperationCount();
        Node* node = search(root.get(), key);
        if (node) {
            value = node->value;
            return true;
        }
        return false;
    }

    bool contains(const K& key) override {
        this->resetOperationCount();
        return search(root.get(), key) != nullptr;
    }

    size_t size() const override { return nodeCount; }
    bool empty() const override { return root == nullptr; }
    void clear() override {
        root.reset();
        nodeCount = 0;
    }

    std::string getName() const override { return "AVL Tree Table"; }
};

#endif