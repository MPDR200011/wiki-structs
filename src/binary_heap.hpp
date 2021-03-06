#pragma once

#include <vector>
#include <functional>
#include <stdexcept>
#include <iostream>

template<class T, typename Comparator = std::less<T>>
class BinaryHeap {
private:
    typedef std::vector<T> Container;

    Container container;
    Comparator compare;

    using ConstReference = typename Container::const_reference;

    inline size_t calcParentIndex(size_t childIndex) {
        return (childIndex-1)/2;
    }

    inline size_t calcChildrenIndex(size_t parentIndex) {
        return parentIndex*2+1;
    }

    void bubbleUp(size_t index) {
        size_t childIndex = index;
        size_t parentIndex = calcParentIndex(childIndex);
        while (childIndex != 0 && compare(container[childIndex], container[parentIndex])) {
            std::swap(container[parentIndex], container[childIndex]);
            childIndex = parentIndex;
            parentIndex = calcParentIndex(childIndex);
        }
    }

    void bubbleDown() {
        unsigned long parentIndex = 0;
        unsigned long childrenIndex = calcChildrenIndex(parentIndex);

        size_t containerSize = container.size();
        bool swapped;
        do {
            unsigned long minIndex = parentIndex;
            if (childrenIndex < containerSize) {
                minIndex = childrenIndex;

                if (childrenIndex+1 < containerSize) {
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

    inline T& unsafePeek() {
        return container[0];
    }
public:
    BinaryHeap() = default;

    void push(const T &val) {
        size_t childIndex = container.size();
        container.push_back(val);

        bubbleUp(childIndex);
    }

    ConstReference peek() {
        if (empty()) {
            throw std::out_of_range("Heap is empty.");
        }

        return unsafePeek();
    }

    void pop() {
        if (empty()) {
            throw std::out_of_range("Heap is empty.");
        }

        std::iter_swap(container.begin(), container.end()-1);
        container.pop_back();

        bubbleDown();
    }

    T pushPop(T val) {
        if (!empty() && !compare(val, unsafePeek())) {
            std::swap(unsafePeek(), val);
            bubbleDown();
        }

        return val;
    }

    T replace(T val) {
        if (empty()) {
            throw std::out_of_range("Heap is empty.");
        }

        std::swap(unsafePeek(), val);
        bubbleDown();

        return val;
    }

    size_t size() {
        return container.size();
    }

    bool empty() {
        return container.empty();
    }
};
