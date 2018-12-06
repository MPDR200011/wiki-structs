#include <iostream>
#include "BinaryHeap.h"
#include <vector>

using namespace std;

int main() {
    BinaryHeap<int> queue;

    queue.push(4);
    queue.push(2);
    queue.push(10);
    queue.push(-3);
    queue.push(1);
    queue.push(3);
    queue.push(5);
    queue.push(-5);
    queue.push(4);
    queue.push(-3);

    vector<int> ordered;
    bool queueNotEmpty = true;
    while (queueNotEmpty) {
        try {
            ordered.push_back(queue.pop());
        } catch (out_of_range &e) {
            queueNotEmpty = false;
        }
    }

    for (const int i: ordered) {
        cout << i << " ";
    }
    cout << endl;

    return 0;
}