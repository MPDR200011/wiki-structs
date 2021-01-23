#include "fibonacci_heap.hpp"
#include "catch2/catch.hpp"
#include <iostream>
#include <typeinfo>

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

TEST_CASE("can't peek empty heap", "[fibonacci_heap]") {
    fibonacci_heap<int> heap;
    try {
        heap.peek();
        REQUIRE(false);
    } catch(...) {
        REQUIRE(true);
    }
}

TEST_CASE("can't pop empty heap", "[fibonacci_heap]") {
    fibonacci_heap<int> heap;
    heap.push(1);
    heap.pop();
    try {
        heap.pop();
        REQUIRE(false);
    } catch(...) {
        REQUIRE(true);
    }
}

struct TestStruct {
    int value;

    bool operator<(const TestStruct& rhs) const {
        return value < rhs.value;
    }

    bool operator==(const TestStruct& rhs) const {
        return value == rhs.value;
    }
};

SCENARIO("fibonacci heap does proper decrease key", "[fibonacci_heap]") {

    GIVEN("binary heap has some items") {
        fibonacci_heap<TestStruct> heap;

        heap.push(TestStruct{2});
        heap.push(TestStruct{3});
        heap.push(TestStruct{-1});
        heap.push(TestStruct{5});
        heap.push(TestStruct{-4});

        TestStruct variable{10};
        fibonacci_heap<TestStruct>::NodeHandle handle = heap.push(variable);

        REQUIRE(heap.peek().value == -4);

        WHEN("key of an item is reduced to the lowest") {
            heap.decrease_key(handle, TestStruct{-10});

            THEN("it becomes the new lowest") {
                REQUIRE(heap.peek().value == -10);
            }
        }
    }

    GIVEN("ginary heap has some pointers to items") {
        struct TestStrcutPtrCmp {
            bool operator() (const TestStruct* lhs, const TestStruct* rhs) const {
                return lhs->value < rhs->value;
            }
        };

        fibonacci_heap<TestStruct*, TestStrcutPtrCmp> heap;

        heap.push(new TestStruct{2});
        heap.push(new TestStruct{3});
        heap.push(new TestStruct{-1});
        heap.push(new TestStruct{5});

        TestStruct* lowest = new TestStruct{-4};
        heap.push(lowest);

        TestStruct* variable = new TestStruct{10};
        fibonacci_heap<TestStruct*, TestStrcutPtrCmp>::NodeHandle handle = heap.push(variable);

        REQUIRE(heap.peek() == lowest);
        REQUIRE(heap.peek()->value == -4);

        WHEN("key of an item is reduced through the pointer") {
            variable->value = -10;
            heap.decrease_key(handle);

            THEN("it becomes the new lowest, and the returned pointer is the same") {
                REQUIRE(heap.peek() == variable);
                REQUIRE(heap.peek()->value == -10);
            }
        }
    }
}

TEST_CASE("can't decrease key to higher value", "[fibonacci_heap]") {
    fibonacci_heap<TestStruct> heap;
    fibonacci_heap<TestStruct>::NodeHandle handle = heap.push(TestStruct{1});

    heap.decrease_key(handle, TestStruct{-1});
    try {
        heap.decrease_key(handle, TestStruct{10});
        REQUIRE(false);
    } catch(...) {
        REQUIRE(true);
    }
}
