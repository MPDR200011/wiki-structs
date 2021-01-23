#pragma once 

#include <iostream>
#include <vector>
#include <functional>
#include <memory>
#include <stdexcept>
#include <cmath>

template <typename T, typename Comparator = std::less<T>>
class fibonacci_heap {
    struct Node;
public:
    typedef std::shared_ptr<Node> NodeHandle;
private:

    static void linkNodes(NodeHandle left, NodeHandle right) {
        left->m_nextSibling = right;
        right->m_prevSibling = left;
    }

    struct Node {
        T m_value;
        NodeHandle m_parent;
        NodeHandle m_nextSibling;
        NodeHandle m_prevSibling;
        NodeHandle m_firstChild;
        size_t m_degree;
        bool m_marked;

        Node(T& value) : m_value(value), m_degree{0}, m_marked{false} {}

        size_t degree() { return m_degree; }

        void addChild(NodeHandle& newChild) {
            if (m_firstChild) {
                linkNodes(m_firstChild->m_prevSibling, newChild);
                linkNodes(newChild, m_firstChild);
            } else {
                m_firstChild = newChild;
                linkNodes(m_firstChild, m_firstChild);
            }
            ++m_degree;
        }

        void removeChild(NodeHandle& child) {
            link(child->m_prevSibling, child->m_nextSibling);

            if (child == m_firstChild) {
                // if we are removing the first child, set the next sibling as new first child
                m_firstChild = m_firstChild->m_nextSibling;

                if (m_firstChild == child) {
                    // if after that nothing changed, then it means that we are trying to remove the only existing
                    // child, if so, reset the first child
                    m_firstChild.reset();
                }
            }
            --m_degree;
        }
    };

    NodeHandle m_listHead;
    NodeHandle m_minimumValueNode;
    Comparator compare;
    size_t m_size;

    void cutNode(NodeHandle& node) {
        NodeHandle parent = node->m_parent;

        // cut node from parent
        node->m_parent.reset();

        // make it a root
        linkNodes(m_listHead->m_prevSibling, node);
        linkNodes(node, m_listHead);
        node->m_marked = false;

        // mark parent
        bool parentMarked = parent->m_marked;
        parent->m_marked = true;
        if (parentMarked) {
            //if previously marked cut it as well
            cutNode(parent);
        }
    }

    void makeNodeRoot(NodeHandle& node) {
        linkNodes(m_listHead->m_prevSibling, node);
        linkNodes(node, m_listHead);
        node->m_parent.reset();
        node->m_marked = false;
    }

public:
    fibonacci_heap() : m_size{0} {}

    size_t size() { return m_size; }

    bool empty() { return m_size == 0; }

    NodeHandle push(T value) {
        /* Operation insert works by creating a new heap with one element and doing merge.
         * This takes constant time, and the potential increases by one, because the number of trees increases.
         * The amortized cost is thus still constant.*/
        if (m_size == 0) {
            m_listHead = std::make_shared<Node>(value);
            m_minimumValueNode = m_listHead;
            linkNodes(m_listHead, m_listHead);
            ++m_size;
            return m_minimumValueNode;
        }

        NodeHandle newNode = std::make_shared<Node>(value);
        linkNodes(m_listHead->m_prevSibling, newNode);
        linkNodes(newNode, m_listHead);
        ++m_size;

        if (compare(newNode->m_value, m_minimumValueNode->m_value)) {
            m_minimumValueNode = newNode;
        }

        return newNode;
    }
    const T& peek() {
        /* Operation find minimum is now trivial because we keep the pointer to the node containing it. 
         * It does not change the potential of the heap, therefore both actual and amortized cost are constant.*/
        if (empty()) {
            throw std::out_of_range("Heap is empty.");
        }

        return m_minimumValueNode->m_value;
    }

    void pop() {
        if (empty()) throw std::out_of_range("Heap is empty.");
        /* Operation extract minimum (same as delete minimum) operates in three phases.
         * First we take the root containing the minimum element and remove it. 
         * Its children will become roots of new trees. 
         */
        if (m_minimumValueNode->degree() > 0) {
            NodeHandle current = m_minimumValueNode->m_firstChild;
            do {
                auto next = current->m_nextSibling;
                makeNodeRoot(current);
                current = next;
            } while (current != m_minimumValueNode->m_firstChild);
        } 

        linkNodes(m_minimumValueNode->m_prevSibling, m_minimumValueNode->m_nextSibling);

        // change list head if it was the minimum value node
        if (m_minimumValueNode == m_listHead) {
            m_listHead = m_minimumValueNode->m_nextSibling;
        }

        m_minimumValueNode.reset();
        --m_size;

        if (empty()) {
            m_listHead.reset();
            return;
        };

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
            NodeHandle curr = m_listHead;
            do {
                size_t degree = curr->degree();
                NodeHandle next = curr->m_nextSibling; // prefetch the next node
                if (!degrees[degree]) {
                    degrees[degree] = curr;
                } else if (degrees[degree] != curr) {
                    // the value in the vector might be the actual current node from a past iteration
                    check = true;

                    NodeHandle bigger;
                    NodeHandle smaller;

                    // check which is smaller
                    if (compare(degrees[degree]->m_value, curr->m_value)) {
                        bigger = curr;
                        smaller = degrees[degree];
                    } else {
                        bigger = degrees[degree];
                        smaller = curr;
                    }


                    linkNodes(bigger->m_prevSibling, bigger->m_nextSibling); // unlink bigger from list
                    smaller->addChild(bigger); // add it to children of smaller
                    bigger->m_parent = smaller; // set the parent of bigger

                    degrees[degree].reset(); // reset the value in the vector as the node's degree has been updated

                    if (bigger == m_listHead) {
                        // we might be deleting the head from the list, it needs to be updated
                        m_listHead = smaller->m_nextSibling;
                    }

                    if (bigger == next) {
                        // next node might have been the one moved, so it needs to be replaced
                        next = m_listHead;
                    }
                }
                curr = next;
            } while (curr != m_listHead);
        }

        NodeHandle minimumNode = m_listHead;
        NodeHandle current = m_listHead;
        do {
            if (compare(current->m_value, minimumNode->m_value)) {
                minimumNode = current;
            }
            current = current->m_nextSibling;
        } while (current != m_listHead);

        m_minimumValueNode = minimumNode;
    }

    void decrease_key(NodeHandle& node) {
        /* Operation decrease key will take the node, decrease the key and if the heap property becomes violated 
         * (the new key is smaller than the key of the parent), the node is cut from its parent. If the parent is 
         * not a root, it is marked. If it has been marked already, it is cut as well and its parent is marked.
         * We continue upwards until we reach either the root or an unmarked node. Now we set the minimum pointer 
         * to the decreased value if it is the new minimum. 
         * Unmark nodes as they becomes roots.
         */
        if (node->m_parent) {
            if (compare(node->m_value, node->m_parent->m_value)) {
                // node now violates the heap roperty
                cutNode(node);
            }
        }

        if (compare(node->m_value, m_minimumValueNode->m_value)) {
            m_minimumValueNode = node;
        }
    }

    void decrease_key(NodeHandle& node, T newValue) {
        if (compare(node->m_value, newValue)) {
            throw std::invalid_argument("deacrese_key: New key value is higher than alredy existing.");
        }

        node->m_value = newValue;
        decrease_key(node);
    }
};
