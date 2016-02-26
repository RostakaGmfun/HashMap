#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "HashMap.hpp"


TEST_CASE( "LinkedList Test", "[LinkedList]") {

    SECTION("Basic test") {
        LinkedList<int> list;
        REQUIRE( list.isEmpty() == true );

        const int numElements = 1000;

        std::string expected;
        int err = 0;
        for(int i = 0;i<numElements;i++) {
            list.pushBack(i);
            expected+=std::to_string(i);
            if(list[i] != i ) {
                err++;
            }
        }
        CHECK( err == 0 );
        REQUIRE( list.size() == numElements );

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

    SECTION("Array resize") {
        Array<int> array;
        REQUIRE( array.size() == 0 );
        REQUIRE( array.capacity() != 0 );

        array.resize(0);
        REQUIRE( array.capacity() == 0 );
        REQUIRE( array.data() == nullptr );

        array.pushBack(42);
        REQUIRE( array.data() != nullptr );
        REQUIRE( array.capacity() != 0 );
        REQUIRE (array.size() == 1 );

        array.resize(100);
        REQUIRE( array.capacity() == 100);
        REQUIRE( array.size() == 1 );
        REQUIRE( array[0] == 42 );

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

    SECTION("HashMap basic test") {
        HashMap<std::string, std::string> hashMap;
        REQUIRE( hashMap.size() == 0 );
        REQUIRE( hashMap.capacity() > 0 );

        hashMap["key1"] = "value1";
        REQUIRE( hashMap.size() == 1);
        hashMap["key1"] = "newValue";
        REQUIRE( hashMap.size() == 1);

        hashMap["key2"] = "value2";
        REQUIRE( hashMap.size() == 2);

        REQUIRE( hashMap["key1"] == "newValue" );
        REQUIRE( hashMap["key2"] == "value2" );
    }

    SECTION("HashMapIterator test") {
        HashMap<std::string, std::string> hashMap;
        REQUIRE( hashMap.size() == 0 );
        REQUIRE( hashMap.capacity() > 0 );

        const int numElem = 100;

        std::string expected;
        for(int i = 0;i<numElem;i++) {
            std::string k = "key"+std::to_string(i);
            //hashMap[k] = "value";
            //expected+=std::string("value"+std::to_string(i));
        }

        std::string got;
        for(auto &it : hashMap) {
            std::cout << it.value << std::endl;
            got+=it.value;
        }

        REQUIRE( got == expected );
        
    }
}


