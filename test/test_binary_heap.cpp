#include "binary_heap.hpp"
#include "catch2/catch.hpp"
#include <iostream>

SCENARIO("binary heap changes size", "[binary_heap]") {
    GIVEN("binary heap has some items") {
        BinaryHeap<int> heap;

        heap.push(4);
        heap.push(2);
        heap.push(10);
        heap.push(-3);
        heap.push(1);

        REQUIRE( heap.size() == 5 );

        WHEN("items are removed") {
            heap.pop();
            THEN("the size changes") {
                REQUIRE( heap.size() == 4 );
            }
        }

        WHEN("items are added") {
            heap.push(6);
            THEN("the size changes") {
                REQUIRE( heap.size() == 6 );
            }
        }
    }
}

SCENARIO("binary heap sorts items", "[binary_heap]") {
    GIVEN("binary heap has some items") {
        BinaryHeap<int> heap;

        heap.push(4);
        heap.push(2);
        heap.push(10);
        heap.push(-3);
        heap.push(1);

        REQUIRE( heap.peek() == -3 );
        
        WHEN("new min is added") {
            heap.push(-4);
            THEN("the heaps top changes") {
                REQUIRE( heap.peek() == -4 );
            }
        }

        WHEN("non min value is added") {
            heap.push(11);
            THEN("the heaps top doesn't change") {
                REQUIRE( heap.peek() == -3 );
            }
        }

        WHEN("value is poped from the heap") {
            heap.pop();
            THEN("the heaps top changes") {
                REQUIRE( heap.peek() == 1 );
            }
        }

        WHEN("all values are popped from the heap") {
            std::vector<int> popped;
            while (!heap.empty()) {
                popped.push_back(heap.peek());
                heap.pop();
            }
            THEN("they come out in ascending order") {
                std::vector<int> test = {-3, 1, 2, 4, 10};
                REQUIRE(std::equal(test.begin(), test.end(), popped.begin()));
            }
        }
    }
}

SCENARIO("binary heap puhs-pop and pop-push/replace work correctly", "[binary_heap]") {
    GIVEN("binary heap has some items") {
        BinaryHeap<int> heap;

        heap.push(4);
        heap.push(2);
        heap.push(10);
        heap.push(-3);
        heap.push(1);

        REQUIRE( heap.peek() == -3 );
        
        WHEN("new minimum is push-popped") {
            THEN("the same value is returned") {
                REQUIRE( heap.pushPop(-4) == -4 );
                REQUIRE( heap.peek() == -3 );
            }
        }

        WHEN("a value that is not lower than the top is push-popped") {
            THEN("the returned value is the original minimum") {
                REQUIRE( heap.pushPop(-2) == -3 );
                REQUIRE( heap.peek() == -2 );
            }
        }

        WHEN("a new minimum is pop-pushed") {
            THEN("the returned value is the original min") {
                REQUIRE( heap.replace(-4) == -3 );
                REQUIRE( heap.peek() == -4 );
                REQUIRE( heap.replace(-4) == -4 );
                REQUIRE( heap.peek() == -4 );
            }
        }

        WHEN("a value that is not lower than the top is pop-pushed") {
            THEN("the returned value is the original min") {
                REQUIRE( heap.replace(6) == -3 );
                REQUIRE( heap.peek() == 1 );
            }
        }
    }
}

TEST_CASE("Cannot peek, pop, or pop-push/replace on an empty Heap.", "[binary_heap]") {
    BinaryHeap<int> heap;

    try {
        heap.peek();
        REQUIRE( false );
    } catch (...) {
        REQUIRE( true );
    }

    try {
        heap.pop();
        REQUIRE( false );
    } catch (...) {
        REQUIRE( true );
    }

    try {
        heap.replace(0);
        REQUIRE( false );
    } catch (...) {
        REQUIRE( true );
    }
}
