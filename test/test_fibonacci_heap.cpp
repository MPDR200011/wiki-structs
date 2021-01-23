#include "fibonacci_heap.hpp"
#include "catch2/catch.hpp"
#include <iostream>

SCENARIO("heap has correct size", "[fibonacci_heap]") {
    GIVEN("binary heap has some items") {
        fibonacci_heap<int> heap;

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

SCENARIO("heap sorts items", "[fibonacci_heap]") {
    GIVEN("binary heap has some items") {
        fibonacci_heap<int> heap;

        heap.push(4);
        heap.push(2);
        heap.push(-3);
        heap.push(1);
        heap.push(10);

        REQUIRE( heap.size() == 5 );

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
            THEN("the heap's top changes") {
                REQUIRE( heap.peek() == 1 );
            }
        }

        WHEN("all values are popped from the heap") {
            std::vector<int> popped;
            while (!heap.empty()) {
                int temp = heap.peek();
                std::cout << temp << std::endl;
                popped.push_back(temp);
                heap.pop();
            }
            THEN("they come out in ascending order") {
                std::vector<int> test = {-3, 1, 2, 4, 10};
                REQUIRE(std::equal(test.begin(), test.end(), popped.begin()));
            }
        }
    }
}
