#include "binary_heap.hpp"
#include "catch2/catch.hpp"
#include <iostream>

SCENARIO("binary heap changes size", "[binary_heap]") {
    GIVEN("binary heap has some items") {
        binary_heap<int> heap;

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
        binary_heap<int> heap;

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
        binary_heap<int> heap;

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
    binary_heap<int> heap;

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

SCENARIO("Push-pop and pop-push work correctly.", "[binary_heap]") {
    struct TestStruct {
        std::string value;
        int key;

        bool operator<(const TestStruct & s2) const {
            return key < s2.key;
        }
    };

    GIVEN("A binary heap has some elements") {
        binary_heap<TestStruct> heap;

        heap.push(TestStruct{.value = "2", .key=2});
        heap.push(TestStruct{.value = "5", .key=5});
        heap.push(TestStruct{.value = "7", .key=7});
        heap.push(TestStruct{.value = "11", .key=11});
        heap.push(TestStruct{.value = "16", .key=16});

        REQUIRE( heap.peek().value == "2" );

        WHEN("A new minimum is push-popped") {
            auto res = heap.pushPop(TestStruct{.value = "1000", .key=1});
            THEN("The same value is returned.") {
                REQUIRE( res.value == "1000" );
            }
        }

        WHEN("A value with same key as minimum is push-popped.") {
            auto res = heap.pushPop(TestStruct{.value = "1000", .key=2});
            THEN("The original minimum is returned.") {
                REQUIRE( res.value == "2" );
            }
        }

        WHEN("A value that is not lower than the minimum is push-popped.") {
            auto res = heap.pushPop(TestStruct{.value = "1000", .key=8});
            THEN("The original minimum is returned.") {
                REQUIRE( res.value == "2" );
            }
        }
    }
}
