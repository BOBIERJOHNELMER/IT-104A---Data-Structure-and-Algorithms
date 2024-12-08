#pragma once
#include <vector>
#include <utility>  // For std::pair

class BinaryTree {
    struct Node {
        int key;
        std::string data;
        Node* left;
        Node* right;
        Node(int k, const std::string& d) : key(k), data(d), left(nullptr), right(nullptr) {}
    };

    Node* root;

public:
    BinaryTree() : root(nullptr) {}

    void insert(int key, const std::string& data);
    std::vector<std::pair<int, int>> getNodePositions();  // Declaration here
};
