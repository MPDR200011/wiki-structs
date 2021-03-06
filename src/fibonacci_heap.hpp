#pragma once 

#include <iostream>
#include <vector>
#include <functional>
#include <memory>
#include <stdexcept>
#include <cmath>

template <typename T, typename Comparator = std::less<T>>
class FibonacciHeap {
    struct Node;
public:
    typedef std::shared_ptr<Node> NodeHandle;
private:

    struct Node {
        T value;
        NodeHandle parent;
        NodeHandle nextSibling;
        NodeHandle prevSibling;
        NodeHandle firstChild;
        size_t degree;
        bool marked;

        Node(T& value) : value(value), degree{0}, marked{false} {}

        size_t getDegree() { return degree; }

        void addChild(NodeHandle& newChild) {
            if (firstChild) {
                linkNodes(firstChild->prevSibling, newChild);
                linkNodes(newChild, firstChild);
            } else {
                firstChild = newChild;
                linkNodes(firstChild, firstChild);
            }
            ++degree;
        }

        void removeChild(NodeHandle& child) {
            link(child->prevSibling, child->nextSibling);

            if (child == firstChild) {
                // if we are removing the first child, set the next sibling as new first child
                firstChild = firstChild->nextSibling;

                if (firstChild == child) {
                    // if after that nothing changed, then it means that we are trying to remove the only existing
                    // child, if so, reset the first child
                    firstChild.reset();
                }
            }
            --degree;
        }
    };

    static void linkNodes(NodeHandle left, NodeHandle right) {
        left->nextSibling = right;
        right->prevSibling = left;
    }

    NodeHandle listHead;
    NodeHandle minimumValueNode;
    Comparator compare;
    size_t m_size;

    void cutNode(NodeHandle& node) {
        NodeHandle parent = node->parent;

        // cut node from parent
        node->parent.reset();

        // make it a root
        linkNodes(listHead->prevSibling, node);
        linkNodes(node, listHead);
        node->marked = false;

        // mark parent
        bool parentMarked = parent->marked;
        parent->marked = true;
        if (parentMarked) {
            //if previously marked cut it as well
            cutNode(parent);
        }
    }

    void makeNodeRoot(NodeHandle& node) {
        linkNodes(listHead->prevSibling, node);
        linkNodes(node, listHead);
        node->parent.reset();
        node->marked = false;
    }

public:
    FibonacciHeap() : m_size{0} {}

    size_t size() { return m_size; }

    bool empty() { return m_size == 0; }

    NodeHandle push(T value) {
        /* Operation insert works by creating a new heap with one element and doing merge.
         * This takes constant time, and the potential increases by one, because the number of trees increases.
         * The amortized cost is thus still constant.*/
        if (m_size == 0) {
            listHead = std::make_shared<Node>(value);
            minimumValueNode = listHead;
            linkNodes(listHead, listHead);
            ++m_size;
            return minimumValueNode;
        }

        NodeHandle newNode = std::make_shared<Node>(value);
        linkNodes(listHead->prevSibling, newNode);
        linkNodes(newNode, listHead);
        ++m_size;

        if (compare(newNode->value, minimumValueNode->value)) {
            minimumValueNode = newNode;
        }

        return newNode;
    }
    const T& peek() {
        /* Operation find minimum is now trivial because we keep the pointer to the node containing it. 
         * It does not change the potential of the heap, therefore both actual and amortized cost are constant.*/
        if (empty()) {
            throw std::out_of_range("Heap is empty.");
        }

        return minimumValueNode->value;
    }

    void pop() {
        if (empty()) throw std::out_of_range("Heap is empty.");
        /* Operation extract minimum (same as delete minimum) operates in three phases.
         * First we take the root containing the minimum element and remove it. 
         * Its children will become roots of new trees. 
         */
        if (minimumValueNode->getDegree() > 0) {
            NodeHandle current = minimumValueNode->firstChild;
            do {
                auto next = current->nextSibling;
                makeNodeRoot(current);
                current = next;
            } while (current != minimumValueNode->firstChild);
        } 

        linkNodes(minimumValueNode->prevSibling, minimumValueNode->nextSibling);

        // change list head if it was the minimum value node
        if (minimumValueNode == listHead) {
            listHead = minimumValueNode->nextSibling;
        }

        minimumValueNode.reset();
        --m_size;

        if (empty()) {
            listHead.reset();
            return;
        }

        /* However to complete the extract minimum operation, we need to update the pointer to the root with 
         * minimum key. Unfortunately there may be up to n roots we need to check. In the second phase we therefore 
         * decrease the number of roots by successively linking together roots of the same degree. 
         * When two roots u and v have the same degree, we make one of them a child of the other so that the one 
         * with the smaller key remains the root. Its degree will increase by one. This is repeated until every root 
         * has a different degree. To find trees of the same degree efficiently we use an array of length O(log n) in 
         * which we keep a pointer to one root of each degree. When a second root is found of the same degree, the two 
         * are linked and the array is updated.
         */

        // reduce number of nodes
        size_t size =  std::log2(m_size) + 1;
        std::vector<NodeHandle> degrees{size};
        bool check = true;
        while (check) {
            check = false;
            NodeHandle curr = listHead;
            do {
                size_t degree = curr->getDegree();
                NodeHandle next = curr->nextSibling; // prefetch the next node
                if (!degrees[degree]) {
                    degrees[degree] = curr;
                } else if (degrees[degree] != curr) {
                    // the value in the vector might be the actual current node from a past iteration
                    check = true;

                    NodeHandle bigger;
                    NodeHandle smaller;

                    // check which is smaller
                    if (compare(degrees[degree]->value, curr->value)) {
                        bigger = curr;
                        smaller = degrees[degree];
                    } else {
                        bigger = degrees[degree];
                        smaller = curr;
                    }


                    linkNodes(bigger->prevSibling, bigger->nextSibling); // unlink bigger from list
                    smaller->addChild(bigger); // add it to children of smaller
                    bigger->parent = smaller; // set the parent of bigger

                    degrees[degree].reset(); // reset the value in the vector as the node's degree has been updated

                    if (bigger == listHead) {
                        // we might be deleting the head from the list, it needs to be updated
                        listHead = smaller->nextSibling;
                    }

                    if (bigger == next) {
                        // next node might have been the one moved, so it needs to be replaced
                        next = listHead;
                    }
                }
                curr = next;
            } while (curr != listHead);
        }

        NodeHandle minimumNode = listHead;
        NodeHandle current = listHead;
        do {
            if (compare(current->value, minimumNode->value)) {
                minimumNode = current;
            }
            current = current->nextSibling;
        } while (current != listHead);

        minimumValueNode = minimumNode;
    }

    void decreaseKey(NodeHandle& node) {
        /* Operation decrease key will take the node, decrease the key and if the heap property becomes violated 
         * (the new key is smaller than the key of the parent), the node is cut from its parent. If the parent is 
         * not a root, it is marked. If it has been marked already, it is cut as well and its parent is marked.
         * We continue upwards until we reach either the root or an unmarked node. Now we set the minimum pointer 
         * to the decreased value if it is the new minimum. 
         * Unmark nodes as they becomes roots.
         */
        if (node->parent) {
            if (compare(node->value, node->parent->value)) {
                // node now violates the heap roperty
                cutNode(node);
            }
        }

        if (compare(node->value, minimumValueNode->value)) {
            minimumValueNode = node;
        }
    }

    void decreaseKey(NodeHandle& node, T newValue) {
        if (compare(node->value, newValue)) {
            throw std::invalid_argument("decreaseKey: New key value is higher than alredy existing.");
        }

        node->value = newValue;
        decreaseKey(node);
    }
};
