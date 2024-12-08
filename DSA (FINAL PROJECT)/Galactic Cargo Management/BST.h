#pragma once
#include <iostream>
#include <string>

class BST {
    struct Node {
        int key;
        std::string data;
        Node* left;
        Node* right;
        Node(int k, const std::string& d) : key(k), data(d), left(nullptr), right(nullptr) {}
    };

    Node* root;

    // Recursive helper for insertion
    Node* insertRec(Node* node, int key, const std::string& data) {
        if (!node) return new Node(key, data);
        if (key < node->key)
            node->left = insertRec(node->left, key, data);
        else if (key > node->key)
            node->right = insertRec(node->right, key, data);
        else
            std::cout << "Duplicate key ignored: " << key << "\n";
        return node;
    }

    // Recursive helper for in-order traversal
    void inorderRec(Node* node) const {
        if (node) {
            inorderRec(node->left);
            std::cout << "Treasure: " << node->data << " (Value: " << node->key << ")\n";
            inorderRec(node->right);
        }
    }

public:
    BST() : root(nullptr) {}

    void insert(int key, const std::string& data) {
        root = insertRec(root, key, data);
    }

    void display() const {
        if (root) {
            inorderRec(root);
        }
        else {
            std::cout << "The inventory is empty.\n";
        }
    }
};
