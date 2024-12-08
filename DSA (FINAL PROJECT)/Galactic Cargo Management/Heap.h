#pragma once
#include <vector>
#include <iostream>
#include <algorithm>

class Heap {
    std::vector<int> heap; // Default is Min-Heap

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index] < heap[parent]) {
                std::swap(heap[index], heap[parent]);
                index = parent;
            }
            else {
                break;
            }
        }
    }

    void heapifyDown(int index) {
        int size = heap.size();
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = index;

            if (left < size && heap[left] < heap[smallest]) smallest = left;
            if (right < size && heap[right] < heap[smallest]) smallest = right;

            if (smallest != index) {
                std::swap(heap[index], heap[smallest]);
                index = smallest;
            }
            else {
                break;
            }
        }
    }

public:
    void insert(int priority) {
        heap.push_back(priority);
        heapifyUp(heap.size() - 1);
    }

    int pop() {
        if (heap.empty()) {
            std::cerr << "Error: No challenges left.\n";
            return -1;
        }
        int root = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);
        return root;
    }

    void display() const {
        if (heap.empty()) {
            std::cout << "No challenges available.\n";
        }
        else {
            std::cout << "Current Challenges: ";
            for (int val : heap) {
                std::cout << val << " ";
            }
            std::cout << "\n";
        }
    }
};
