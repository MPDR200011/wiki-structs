#ifndef BINARYHEAP_BINARYHEAP_H
#define BINARYHEAP_BINARYHEAP_H

#include <vector>
#include <functional>
#include <stdexcept>
#include <iostream>

template<class T> class BinaryHeap {
private:
    std::vector<T> container;
    std::function<bool(const T&, const T&)> compare;
public:
    BinaryHeap();
    explicit BinaryHeap(std::function<bool(const T&, const T&)> comp);

    void push(const T &val);
    T peek();
    T pop();

    size_t size();
    bool empty();
};

template <class T>
BinaryHeap<T>::BinaryHeap() {
    this->compare = [](const T &t1, const T &t2) -> bool {
        return t1 < t2;
    };
}

template <class T>
BinaryHeap<T>::BinaryHeap(std::function<bool(const T &, const T&)> comp) {
    this->compare = comp;
}

template<class T>
void BinaryHeap<T>::push(const T &val) {
    if (empty()) {
        container.push_back(val);
        return;
    }
    unsigned long childIndex = container.size();
    container.push_back(val);
    unsigned long parentIndex = (childIndex-1)/2;
    while (compare(container[childIndex], container[parentIndex]) && childIndex != 0) {
        std::swap(container[parentIndex], container[childIndex]);
        childIndex = parentIndex;
        parentIndex = (childIndex-1)/2;
    }
}

template<class T>
T BinaryHeap<T>::peek() {
    return container[0];
}

template<class T>
T BinaryHeap<T>::pop() {
    if (empty()) {
        throw std::out_of_range("Heap is empty.");
    }

    T res = peek();

    container.erase(container.begin());

    if (empty()) {
        return res;
    }

    T back = container.back();
    container.pop_back();
    container.insert(container.begin(), back);

    unsigned long parentIndex = 0;
    unsigned long childrenIndex = parentIndex*2+1;

    bool swapped;
    do {

        unsigned long minIndex = parentIndex;
        if (childrenIndex < container.size()) {
            minIndex = childrenIndex;
            if (childrenIndex+1 < container.size()) {
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

    return res;
}

template<class T>
size_t BinaryHeap<T>::size() {
    return container.size();
}

template<class T>
bool BinaryHeap<T>::empty() {
    return container.empty();
}

#endif //BINARYHEAP_BINARYHEAP_H
