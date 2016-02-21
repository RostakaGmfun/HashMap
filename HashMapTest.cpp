#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "HashMap.hpp"


TEST_CASE( "LinkedList Test", "[LinkedList]") {

    SECTION("Basic test") {
        LinkedList<int> list;
        REQUIRE( list.isEmpty() == true );

        std::string expected;
        for(int i = 0;i<10;i++) {
            list.pushBack(i);
            expected+=std::to_string(i);
            REQUIRE ( list[i] == i );
        }
        REQUIRE( list.size() == 10 );

        std::string got;
        for(const auto &it : list) {
            got+=std::to_string(it->value); 
        }
        REQUIRE( got == expected );

        list.clear();
        REQUIRE( list.isEmpty() == true );

    }

    SECTION("Remove elements") {
        LinkedList<int> list;
        const int numElements = 100;
        for(int i = 0;i<numElements;i++) {
            list.pushBack(i);
        }

        REQUIRE( list.size() == numElements );

        for(int i = 0;i<numElements/2;i++) {
            list.remove(list.size()-1);
        }

        REQUIRE( list.size() == numElements/2);
        list.clear();
    }

    SECTION("Find") {
        LinkedList<int> list;
        const int numElements = 100;
        for(int i = 0;i<numElements;i++) {
            list.pushBack(i);
        }
        
        // dereference to overcome Catch dumbness
        REQUIRE( *list.find(-1) == *list.end() );
        REQUIRE( list.find(10)->next->value == 11 );

    }
}

