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

TEST_CASE("Array Test", "[Array]") {
    SECTION("Basic Test") {
        Array<int> array;
        REQUIRE( array.size() == 0 );
        REQUIRE( array.data() != nullptr );

        int oldCapacity = array.capacity();

        for(int i = 0;i<oldCapacity;i++) {
            array.pushBack(i);
        }

        REQUIRE( array.capacity() == oldCapacity );
        array.pushBack(42);
        REQUIRE( array.capacity() == oldCapacity*2 );
        array.clear();
        REQUIRE( array.size() == 0 );
    }

    SECTION("Array iteration") {
        Array<int> array;
        REQUIRE( array.size() == 0 );
        REQUIRE( array.data() != nullptr );

        const int numElements = 100;
        
        std::string expected;
        for(int i = 0;i<numElements;i++) {
            array.pushBack(i);
            expected+=std::to_string(i);
        }
        
        std::string got;
        for( const auto &it : array) {
            got+=std::to_string(it);
        }

        REQUIRE( got == expected );
    }
}

