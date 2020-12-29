#pragma once

#include <vector>
#include <functional>
#include <stdexcept>
#include <iostream>

template<class T, typename Comparator = std::less<T>> class BinaryHeap {
private:
    typedef std::vector<T> Container;

    Container container;
    Comparator compare;

    typedef typename Container::const_reference const_reference;

    inline size_t calc_parent_index(size_t child_index) {
        return (child_index-1)/2;
    }

    inline size_t calc_children_index(size_t parent_index) {
        return parent_index*2+1;
    }

    void bubble_up(size_t index) {
        size_t childIndex = index;
        size_t parentIndex = calc_parent_index(childIndex);
        while (compare(container[childIndex], container[parentIndex]) && childIndex != 0) {
            std::swap(container[parentIndex], container[childIndex]);
            childIndex = parentIndex;
            parentIndex = calc_parent_index(childIndex);
        }
    }

    void bubble_down() {
        unsigned long parentIndex = 0;
        unsigned long childrenIndex = calc_children_index(parentIndex);

        size_t container_size = container.size();
        bool swapped;
        do {
            unsigned long minIndex = parentIndex;
            if (childrenIndex < container_size) {
                minIndex = childrenIndex;

                if (childrenIndex+1 < container_size) {
                    if (compare(container[childrenIndex+1],container[minIndex])) {
                        minIndex = childrenIndex+1;
                    }
                }
            }

            if (compare(container[minIndex],container[parentIndex])) {
                std::swap(container[parentIndex], container[minIndex]);
                swapped = true;
                parentIndex = minIndex;
                childrenIndex = parentIndex*2+1;
            } else {
                swapped = false;
            }

        } while (swapped);
    }
public:
    BinaryHeap() {};

    void push(const T &val) {
        size_t childIndex = container.size();
        container.push_back(val);

        bubble_up(childIndex);
    }

    const_reference top() {
        if (empty()) {
            throw std::out_of_range("Heap is empty.");
        }

        return container[0];
    }

    void pop() {
        if (empty()) {
            throw std::out_of_range("Heap is empty.");
        }

        std::iter_swap(container.begin(), container.end()-1);
        container.pop_back();

        bubble_down();
    }

    size_t size() {
        return container.size();
    }

    bool empty() {
        return container.empty();
    }
};
