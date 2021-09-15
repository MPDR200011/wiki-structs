#pragma once

#include <vector>
#include <functional>

template <typename T, typename Comparator = std::less<T>>
class RedBlackTree {
public:
    RedBlackTree() = default;

    size_t size() const;
    bool empty() const;

    void push();
};

