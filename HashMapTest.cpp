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
        REQUIRE( list.find(-1) == list.end() );
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

TEST_CASE("HashMap Test", "[HashMap]") {
    SECTION("hash() Test") {
        const int numHashes = 1000;
        Array<std::size_t> hashes(numHashes);
        int err = 0;

        // test hash for std::string
        const int numChars = 20;
        for(int i = 0;i<numHashes;i++) {
            std::string str;
            for(int j = 0;j<numChars;j++) {
                str.push_back('0'+std::rand()%('z'-'0'));
            }
            std::size_t h = hash(str);
            if(hashes.find(h) != hashes.end()) {
                err++;
            }
            hashes.pushBack(h);
        }
        CHECK( err == 0 );

        // test hash for std::int32_t
        hashes.clear();
        err = 0;
        for(int i = 0;i<numHashes;i++) {
            std::size_t h = hash(std::int32_t(i));
            if(hashes.find(h) != hashes.end()) {
                err++;
            }
            hashes.pushBack(h);
        }
        CHECK( err == 0);

        // test hash for byte array
        hashes.clear();
        err = 0;
        for(int i = 1;i<numHashes+1;i++) {
            Array<std::uint8_t> byteArray(i*2);
            for(int j = 1;j<byteArray.capacity()+1; j++) {
                byteArray.pushBack(std::rand()%255);
            }
            std::size_t h = hash(byteArray);
            if(hashes.find(h) != hashes.end()) {
                err++;
            }
            hashes.pushBack(h);
        }
        CHECK( err == 0);
    }
}


